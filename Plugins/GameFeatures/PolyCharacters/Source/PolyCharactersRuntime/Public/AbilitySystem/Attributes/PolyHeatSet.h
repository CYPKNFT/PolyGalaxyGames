// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "AbilitySystemComponent.h"

#include "PolyHeatSet.generated.h"

/**
 * Wanted/heat-related attributes (crime/notoriety).
 */
UCLASS(BlueprintType)
class POLYCHARACTERSRUNTIME_API UPolyHeatSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:
	UPolyHeatSet();

	ATTRIBUTE_ACCESSORS(UPolyHeatSet, WantedLevel);     // 0..7
	ATTRIBUTE_ACCESSORS(UPolyHeatSet, HeatDecayRate);   // per-second decay
	ATTRIBUTE_ACCESSORS(UPolyHeatSet, CrimeNotoriety);  // long-tail notoriety
	ATTRIBUTE_ACCESSORS(UPolyHeatSet, BribeCostScalar); // scales cost to clear heat

protected:
	UFUNCTION() void OnRep_WantedLevel(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_HeatDecayRate(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_CrimeNotoriety(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_BribeCostScalar(const FGameplayAttributeData& OldValue);

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WantedLevel, Category="Poly|Heat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData WantedLevel;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HeatDecayRate, Category="Poly|Heat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData HeatDecayRate;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CrimeNotoriety, Category="Poly|Heat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CrimeNotoriety;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BribeCostScalar, Category="Poly|Heat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData BribeCostScalar;
};


