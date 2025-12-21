// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "PolyDamageExecution.generated.h"

// Damage execution: consumes SetByCaller.Poly.Damage + one Poly.Damage.* tag; applies power/crit/flat mitigation/block/resistance; outputs to Lyra damage meta so shield/health logic applies. No source-side lifesteal/thorns here—apply via follow-up effects if needed.
UCLASS()
class POLYCHARACTERSRUNTIME_API UPolyDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UPolyDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

