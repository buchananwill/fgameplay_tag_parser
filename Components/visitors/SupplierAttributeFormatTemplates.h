//
// Created by thele on 21/07/2025.
//

#ifndef SUPPLIERATTRIBUTEFORMATTEMPLATES_H
#define SUPPLIERATTRIBUTEFORMATTEMPLATES_H
#include <format>
#include <string>

namespace Templates::Product {
	inline constexpr std::format_string<const std::string &> fragments_template{
		R"(
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
)"
	};

	inline constexpr std::format_string<const std::string &> header_template{
		R"(
/*
* This file is auto-generated and should not be edited.
*/

#pragma once
#include "EconomicSystem/SupplierFragments/SupplierUtilityMixin.h"
#include "CoreMinimal.h"
#include "MassSignalProcessorBase.h"
#include "{0}Processor.generated.h"


UCLASS()
class PISTEPERFECT_API U{0}UtilityProcessor : public UMassSignalProcessorBase
{{
	GENERATED_BODY()

public:
	explicit U{0}UtilityProcessor();

	virtual void Initialize(UObject& Owner) override;

	virtual void ConfigureQueries() override;

protected:
	virtual void SignalEntities(FMassEntityManager& EntityManager, FMassExecutionContext& Context,
		FMassSignalNameLookup& EntitySignals) override;

private:
	FMassEntityQuery EntityQuery;
}};
)"
	};

	inline constexpr std::format_string<const std::string &> cpp_template{
		R"(
/*
* This file is auto-generated and should not be edited.
*/

#include "{0}Processor.h"
#include "{0}Fragments.h"
#include "MassSignalSubsystem.h"
#include "Mass/Globals.h"
#include "Mass/Signals.h"

U{0}UtilityProcessor::U{0}UtilityProcessor(): EntityQuery(*this)
{{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = static_cast<uint8>(EProcessorExecutionFlags::AllWorldModes);
	ExecutionOrder.ExecuteAfter.Add(Resort::Mass::ProcessorGroups::UtilityReset);
	ExecutionOrder.ExecuteInGroup = Resort::Mass::ProcessorGroups::UtilityAccumulation;
}}

void U{0}UtilityProcessor::ConfigureQueries()
{{
	Resort::Mass::Economy::ConfigureQueries<F{0}Attribute, F{0}Archetypes>(EntityQuery);
}}

void U{0}UtilityProcessor::Initialize(UObject& Owner)
{{
	Super::Initialize(Owner);
	UMassSignalSubsystem& SignalSubsystem = *UWorld::GetSubsystem<UMassSignalSubsystem>(Owner.GetWorld());
	SubscribeToSignal(SignalSubsystem, Resort::Mass::Signals::ReComputeUtility);
}}

void U{0}UtilityProcessor::SignalEntities(FMassEntityManager& EntityManager, FMassExecutionContext& Context,
	FMassSignalNameLookup& EntitySignals)
{{
	Resort::Mass::Economy::AccumulateAttributeIntoArchetypeUtility<F{0}Attribute, F{0}Archetypes>(EntityManager, Context, EntityQuery);
}}

)"
	};

	inline constexpr std::format_string<const std::string &> include_wrapper{R"(#include "{0}Fragments.h")"};

	inline constexpr std::format_string<const std::string &, const std::string &> if_attribute_template{
		R"(
		if (Tag == {0})
		{{
			Functor.template operator()<F{1}Attribute>();
			return;
		}}

)"
	};

	inline constexpr std::format_string<const std::string &, const std::string &> if_archetypes_template{
		R"(
		if (Tag == {0})
		{{
			Functor.template operator()<F{1}Archetypes>();
			return;
		}}

)"
	};

	inline constexpr std::format_string<const std::string &, const std::string &, const std::string &>
	tag_to_fragment_dispatch{
		R"(
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
)"
	};

	inline constexpr std::format_string<const std::string &> canonical_supplier_attributes{
		R"(
/*
* This file is auto-generated and should not be edited.
*/

#pragma once
#include "Buildables/BuildableGameplayTags.h"

namespace Resort::Data
{{
	const TArray<FGameplayTag>& SupplierAttributeTagList();

	inline const TArray<FGameplayTag>& SupplierAttributeTagList()
	{{
		static TArray<FGameplayTag> Tags = {{
			{}
		}};
		return Tags;
	}}

}}


)"
	};

	inline constexpr std::format_string<const std::string &> weighting_csv_row{
		R"csvrow({0},"(TagName=""{0}"")","0.000000")csvrow"
	};

	inline const std::string csv_header{"RowName,ProductAttributeTag,Weighting"};
}

#endif //SUPPLIERATTRIBUTEFORMATTEMPLATES_H
