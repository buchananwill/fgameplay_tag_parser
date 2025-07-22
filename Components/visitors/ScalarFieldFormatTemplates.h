//
// Created by thele on 22/07/2025.
//

#ifndef SCALARFIELDFORMATTEMPLATE_H
#define SCALARFIELDFORMATTEMPLATE_H
#include <format>
#include <string>

namespace Templates::ScalarField {
		inline constexpr std::format_string<const std::string &> fragments_template{R"(
/*
* This file is auto-generated and should not be edited.
*/

#pragma once
#include "CoreMinimal.h"
#include "EconomicSystem/SupplierFragments/EconomyConcepts.h"
#include "{0}ScalarFieldFragments.generated.h"

USTRUCT()
struct F{0}Accumulator : public FFieldAccumulatorFragment
{{
	GENERATED_BODY()

}};

USTRUCT()
struct F{0}FieldEmitters : public FScalarFieldEmitterSharedFragment
{{
	GENERATED_BODY()

}};
)"};

	inline constexpr std::format_string<const std::string &> header_template{
		R"(
/*
* This file is auto-generated and should not be edited.
*/

#pragma once
#include "CoreMinimal.h"
#include "MassSignalProcessorBase.h"
#include "{0}Processor.generated.h"


UCLASS()
class PISTEPERFECT_API U{0}FieldAccumulatorProcessor : public UMassSignalProcessorBase
{{
	GENERATED_BODY()

public:
	explicit U{0}FieldAccumulatorProcessor();

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

#include "{0}FieldAccumulatorProcessor.h"
#include "{0}ScalarFieldFragments.h"
#include "EconomicSystem/SupplierFragments/ExtrinsicAttributeUpdate.h"
#include "MassSignalSubsystem.h"
#include "Mass/Globals.h"
#include "Mass/Signals.h"

U{0}FieldAccumulatorProcessor::U{0}FieldAccumulatorProcessor(): EntityQuery(*this)
{{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = static_cast<uint8>(EProcessorExecutionFlags::AllWorldModes);
	ExecutionOrder.ExecuteInGroup = Resort::Mass::ProcessorGroups::ScalarFieldAccumulation;
}}

void U{0}FieldAccumulatorProcessor::ConfigureQueries()
{{
	Resort::Mass::Economy::ConfigureAccumulationQuery<FExampleScalarFieldEmitters, FExampleScalarField>(EntityQuery);
}}

void U{0}FieldAccumulatorProcessor::Initialize(UObject& Owner)
{{
	Super::Initialize(Owner);
	UMassSignalSubsystem& SignalSubsystem = *UWorld::GetSubsystem<UMassSignalSubsystem>(Owner.GetWorld());
	SubscribeToSignal(SignalSubsystem, Resort::Mass::Signals::AccumulateScalarField);
}}

void U{0}FieldAccumulatorProcessor::SignalEntities(FMassEntityManager& EntityManager, FMassExecutionContext& Context,
	FMassSignalNameLookup& EntitySignals)
{{
	Resort::Mass::Economy::AccumulateField<FExampleScalarFieldEmitters, FExampleScalarField>(EntityManager, Context, EntityQuery);
}}

)"
	};

	inline constexpr std::format_string<const std::string &> include_wrapper{R"(#include "{0}ScalarFieldFragments.h")"};

	inline constexpr std::format_string<const std::string &, const std::string &> if_accumulator_template{
		R"(
		if (Tag == {0})
		{{
			Functor.template operator()<F{1}Accumulator>();
			return;
		}}

)"
	};

	inline constexpr std::format_string<const std::string &, const std::string &> if_field_emitters_template{
		R"(
		if (Tag == {0})
		{{
			Functor.template operator()<F{1}FieldEmitters>();
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
	void DispatchAccumulator(const FGameplayTag& Tag, F&& Functor)
	{{
		{1}

		{{
			UE_LOGFMT(LogResort, Fatal, "Unconfigured FGameplayTag received: {{tag}}", Tag.GetTagName());
			checkNoEntry()
		}}
	}}

	template <typename F>
	void DispatchFieldEmitters(const FGameplayTag& Tag, F&& Functor)
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

	inline constexpr std::format_string<const std::string &> canonical_scalar_fields{
		R"(
/*
* This file is auto-generated and should not be edited.
*/

#pragma once
#include "Buildables/BuildableGameplayTags.h"

namespace Resort::Data
{{
	const TArray<FGameplayTag>& ScalarFieldTagList();

	inline const TArray<FGameplayTag>& ScalarFieldTagList()
	{{
		static TArray<FGameplayTag> Tags = {{
			{}
		}};
		return Tags;
	}}

}}


)"
	};

}

#endif //SCALARFIELDFORMATTEMPLATE_H
