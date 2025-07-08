//
// Created by thele on 07/07/2025.
//

#include "gameplay_tag_supplier_processor.h"

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>

#include "flags.h"

inline constexpr std::format_string<const std::string&> header_template{R"(
/*
* This file is auto-generated and should not be edited.
*/

#pragma once
#include "SupplierUtilityMixin.h"
#include "CoreMinimal.h"
#include "{0}Processor.generated.h"

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

UCLASS()
class U{0}UtilityProcessor : public UMassProcessor
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
#include "{0}Processor.h"

void U{0}UtilityProcessor::ConfigureQueries()
{{
	Resort::Mass::Economy::ConfigureQueries<F{0}Attribute, F{0}Archetypes>(EntityQuery);
}}

void U{0}UtilityProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{{
	Resort::Mass::Economy::Execute<F{0}Attribute, F{0}Archetypes>(EntityManager, Context, EntityQuery);
}}

)"};

bool gameplay_tag_supplier_processor::visit_tree(const std::shared_ptr<TagNode> &root) {
	return tag_node_tree_visitor::visit_tree(root);
}

void gameplay_tag_supplier_processor::process_node(const TagNode &node) {
	if (!std::ranges::any_of(node.Flags, [](const std::string &flag) { return flag == visitor::flags::supplier_attribute; })) {
		return;
	}

	std::ofstream header_file(std::format(R"({}Processor.h)", node.Name));
	std::ofstream cpp_file(std::format(R"({}Processor.cpp)", node.Name));

	if (!header_file.is_open() || !cpp_file.is_open()) {
		std::cout << "Couldn't open file\n";
		return;
	}

	auto header_file_content = std::format(header_template, node.Name);
	header_file << header_file_content;
	header_file.close();

	auto cpp_file_content = std::format(cpp_template, node.Name);
	cpp_file << cpp_file_content;
	cpp_file.close();
}
