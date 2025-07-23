//
// Created by thele on 23/07/2025.
//

#ifndef FIELDATTRIBUTEPROCESSORTEMPLATES_H
#define FIELDATTRIBUTEPROCESSORTEMPLATES_H
#include <format>
#include <string>

namespace Templates::FieldAttribute {
		inline constexpr std::format_string<const std::string&> header_template{
		R"(
/*
* This file is auto-generated and should not be edited.
*/

#pragma once
#include "CoreMinimal.h"
#include "MassSignalProcessorBase.h"
#include "{0}FieldAttributeProcessor.generated.h"


UCLASS()
class PISTEPERFECT_API U{0}FieldAttributeProcessor : public UMassSignalProcessorBase
{{
	GENERATED_BODY()

public:
	explicit U{0}FieldAttributeProcessor();

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

	inline constexpr std::format_string<const std::string&, const std::string&> cpp_template{
		R"(
/*
* This file is auto-generated and should not be edited.
*/

#include "{0}FieldAttributeProcessor.h"
#include "{0}ScalarFieldFragments.h"
#include "{0}Fragments.h"
#include "EconomicSystem/SupplierFragments/ExtrinsicAttributeUpdate.h"
#include "MassSignalSubsystem.h"
#include "Mass/Globals.h"
#include "Mass/Signals.h"

U{0}FieldAttributeProcessor::U{0}FieldAttributeProcessor(): EntityQuery(*this)
{{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = static_cast<uint8>(EProcessorExecutionFlags::AllWorldModes);
	ExecutionOrder.ExecuteAfter.Add(Resort::Mass::ProcessorGroups::ScalarFieldAccumulation);
}}

void U{0}FieldAttributeProcessor::ConfigureQueries()
{{
	Resort::Mass::Economy::ConfigureApplyFieldQuery<F{0}Accumulator, F{0}Attribute>(EntityQuery);
}}

void U{0}FieldAttributeProcessor::Initialize(UObject& Owner)
{{
		Super::Initialize(Owner);
	UMassSignalSubsystem& SignalSubsystem = *UWorld::GetSubsystem<UMassSignalSubsystem>(Owner.GetWorld());
	SubscribeToSignal(SignalSubsystem, Resort::Mass::Signals::AccumulateScalarField);
	FieldInflectionVariable = Resort::Mass::Economy::InflectionScalingValue * Resort::Mass::Economy::GetEightyPercentKnee({1});
}}

void U{0}FieldAttributeProcessor::SignalEntities(FMassEntityManager& EntityManager, FMassExecutionContext& Context,
	FMassSignalNameLookup& EntitySignals)
{{
	Resort::Mass::Economy::ApplyFieldToAttribute<F{0}Accumulator, F{0}Attribute>(EntityManager, Context, EntityQuery, FieldInflectionVariable);
}}

)"
	};
}

#endif //FIELDATTRIBUTEPROCESSORTEMPLATES_H
