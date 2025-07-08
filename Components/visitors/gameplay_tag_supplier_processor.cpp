//
// Created by thele on 07/07/2025.
//

#include "gameplay_tag_supplier_processor.h"

#include <algorithm>
#include <iostream>

constexpr auto processor_template = R"(
#pragma once
#include "SupplierUtilityMixin.h"
#include "CoreMinimal.h"
#include "{Token}Processor.generated.h"

USTRUCT()
struct F{Token}Attribute : public FMassFragment
{
	GENERATED_BODY()
	double Value;
};

USTRUCT()
struct F{Token}Archetypes : public FMassConstSharedFragment
{
	GENERATED_BODY()
	TArray<double> Weightings;
};

UCLASS()
class U{Token}UtilityProcessor : public UMassProcessor
{
	GENERATED_BODY()

public:
	explicit U{Token}UtilityProcessor()
		: EntityQuery(*this)
	{
		bAutoRegisterWithProcessingPhases = true;
		ExecutionFlags = static_cast<uint8>(EProcessorExecutionFlags::All);
	}


	virtual void ConfigureQueries() override
	{
		Resort::Mass::Economy::ConfigureQueries<F{Token}Attribute, F{Token}Archetypes>(EntityQuery);
	}

	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override
	{
		Resort::Mass::Economy::Execute<F{Token}Attribute, F{Token}Archetypes>(EntityManager, Context, EntityQuery);
	}

private:
	FMassEntityQuery EntityQuery;
};
)";

constexpr auto supplier_attribute_flag = "SupplierAttribute";

bool gameplay_tag_supplier_processor::visit_tree(const std::shared_ptr<TagNode>& root) {
	return tag_node_tree_visitor::visit_tree(root);
}

void gameplay_tag_supplier_processor::process_node(const TagNode& node) {
	if (!std::ranges::any_of(node.Flags, [](const std::string& flag){ return flag == supplier_attribute_flag;})) {
		std::cout << "Node did not have flag: " << supplier_attribute_flag << std::endl;
	}
}
