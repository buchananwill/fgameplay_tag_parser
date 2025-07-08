//
// Created by thele on 07/07/2025.
//

#include "gameplay_tag_supplier_processor.h"

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>

inline constexpr std::format_string<const std::string&> processor_template{R"(
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


	virtual void ConfigureQueries() override
	{{
		Resort::Mass::Economy::ConfigureQueries<F{0}Attribute, F{0}Archetypes>(EntityQuery);
	}}

	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override
	{{
		Resort::Mass::Economy::Execute<F{0}Attribute, F{0}Archetypes>(EntityManager, Context, EntityQuery);
	}}

private:
	FMassEntityQuery EntityQuery;
}};
)"};

constexpr auto supplier_attribute_flag = "SupplierAttribute";

bool gameplay_tag_supplier_processor::visit_tree(const std::shared_ptr<TagNode> &root) {
	return tag_node_tree_visitor::visit_tree(root);
}

void gameplay_tag_supplier_processor::process_node(const TagNode &node) {
	if (!std::ranges::any_of(node.Flags, [](const std::string &flag) { return flag == supplier_attribute_flag; })) {
		return;
	}

	std::ofstream header_file;
	header_file.open("test.h");

	if (!header_file.is_open()) {
		std::cout << "Couldn't open file\n";
		return;
	}

	auto header_file_content = std::format(processor_template, node.Name);

	header_file << header_file_content;

	header_file.close();
}
