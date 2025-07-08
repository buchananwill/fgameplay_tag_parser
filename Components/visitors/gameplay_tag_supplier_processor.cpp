//
// Created by thele on 07/07/2025.
//

#include "gameplay_tag_supplier_processor.h"

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>

#include "flags.h"
#include "../helpers.h"

namespace fs = std::filesystem;

inline constexpr std::format_string<const std::string&> fragments_template{R"(
/*
* This file is auto-generated and should not be edited.
*/

#pragma once
#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "{0}Fragments.generated.h"

USTRUCT()
struct F{0}Attribute : public FMassFragment
{{
	GENERATED_BODY()
	double Value;
}};

USTRUCT()
struct F{0}Archetypes : public FMassConstSharedFragment
{{
	GENERATED_BODY()
	TArray<double> Weightings;
}};
)"};

inline constexpr std::format_string<const std::string&> header_template{R"(
/*
* This file is auto-generated and should not be edited.
*/

#pragma once
#include "EconomicSystem/SupplierFragments/SupplierUtilityMixin.h"
#include "CoreMinimal.h"
#include "{0}Processor.generated.h"


UCLASS()
class PISTEPERFECT_API U{0}UtilityProcessor : public UMassProcessor
{{
	GENERATED_BODY()

public:
	explicit U{0}UtilityProcessor()
		: EntityQuery(*this)
	{{
		bAutoRegisterWithProcessingPhases = true;
		ExecutionFlags = static_cast<uint8>(EProcessorExecutionFlags::All);
	}}


	virtual void ConfigureQueries() override;

	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
}};
)"};

inline constexpr std::format_string<const std::string&> cpp_template{R"(
/*
* This file is auto-generated and should not be edited.
*/

#include "{0}Processor.h"
#include "{0}Fragments.h"

void U{0}UtilityProcessor::ConfigureQueries()
{{
	Resort::Mass::Economy::ConfigureQueries<F{0}Attribute, F{0}Archetypes>(EntityQuery);
}}

void U{0}UtilityProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{{
	Resort::Mass::Economy::Execute<F{0}Attribute, F{0}Archetypes>(EntityManager, Context, EntityQuery);
}}

)"};

inline constexpr std::format_string<const std::string&> include_wrapper{R"(#include "{0}Fragments.h")"};

inline constexpr std::format_string<const std::string&, const std::string&> if_attribute_template{R"(
		if (Tag == {0})
		{{
			Functor.template operator()<F{1}Attribute>();
			return;
		}}

)"};

inline constexpr std::format_string<const std::string&, const std::string&> if_archetypes_template{R"(
		if (Tag == {0})
		{{
			Functor.template operator()<F{1}Archetypes>();
			return;
		}}

)"};

inline constexpr std::format_string<std::string&, std::string&, std::string&> tag_to_fragment_dispatch{R"(
/*
* This file is auto-generated and should not be edited.
*/

#pragma once
{0}
#include "GameplayTagContainer.h"
#include "Buildables/BuildableGameplayTags.h"
#include "DeveloperTools/Logging.h"
#include "Logging/StructuredLog.h"

namespace Resort::GameplayTag
{{
	template <typename F>
	void DispatchAttribute(const FGameplayTag& Tag, F&& Functor)
	{{
		{1}

		{{
			UE_LOGFMT(LogResort, Fatal, "Unconfigured FGameplayTag received: {{tag}}", Tag.GetTagName());
			checkNoEntry()
		}}
	}}

	template <typename F>
	void DispatchArchetypes(const FGameplayTag& Tag, F&& Functor)
	{{
		{2}

		{{
			UE_LOGFMT(LogResort, Fatal, "Unconfigured FGameplayTag received: {{tag}}", Tag.GetTagName());
			checkNoEntry()
		}}
	}}
}}
)"};

bool gameplay_tag_supplier_processor::visit_tree(const std::shared_ptr<TagNode> &root) {
	header_dir = input_path.parent_path() / "Public";
	cpp_dir = input_path.parent_path() / "Private";

	fs::create_directories(header_dir);
	fs::create_directories(cpp_dir);

	if (!tag_node_tree_visitor::visit_tree(root)) {
		return false;
	};

	if (includes_buffer.empty() || if_attribute_branches_buffer.empty()) {
		return true;
	}

	auto tag_to_dispatch_file_path = header_dir / "TagToFragmentDispatch.h";

	std::ofstream t_to_d_file(tag_to_dispatch_file_path);

	if (!t_to_d_file) {
		std::cout << "Couldn't open tag to dispatch file\n";
		return false;
	}

	t_to_d_file << std::format(tag_to_fragment_dispatch, includes_buffer, if_attribute_branches_buffer, if_archetypes_branches_buffer);
	t_to_d_file.close();

	return true;
}

void gameplay_tag_supplier_processor::process_node(const TagNode &node) {
	if (!std::ranges::any_of(node.Flags, [](const std::string &flag) { return flag == visitor::flags::supplier_attribute; })) {
		return;
	}

	auto fragments_file_path = header_dir / (node.Name + "Fragments.h");
	auto header_file_path = header_dir / (node.Name + "Processor.h");
	auto cpp_file_path = cpp_dir / (node.Name + "Processor.cpp");

	std::ofstream fragments_file(fragments_file_path);
	std::ofstream header_file(header_file_path);
	std::ofstream cpp_file(cpp_file_path);

	if (!header_file || !cpp_file || !fragments_file) {
		std::cout << "Couldn't open file(s)\n";
		return;
	}

	// Buffer strings for later
	{
		includes_buffer.append(std::format(include_wrapper, node.Name));
		includes_buffer.append("\n");

		const auto underscore_name = helpers::join(segments, "_");

		if_attribute_branches_buffer.append(std::format(if_attribute_template, underscore_name, node.Name));
		if_archetypes_branches_buffer.append(std::format(if_archetypes_template, underscore_name, node.Name));
	}

	fragments_file << std::format(fragments_template, node.Name);
	fragments_file.close();

	header_file << std::format(header_template, node.Name);
	header_file.close();

	cpp_file <<  std::format(cpp_template, node.Name);
	cpp_file.close();
}
