// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "PolyDamageExecution.generated.h"

/**
 * Damage execution that applies simple per-type resistance scaling.
 * Expects a SetByCaller damage magnitude tagged with Poly damage type.
 */
UCLASS()
class POLYCHARACTERSRUNTIME_API UPolyDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UPolyDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

