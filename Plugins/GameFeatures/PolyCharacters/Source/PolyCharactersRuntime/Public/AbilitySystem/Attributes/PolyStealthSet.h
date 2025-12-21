// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "AbilitySystemComponent.h"

#include "PolyStealthSet.generated.h"

// Stealth/visibility attributes (stealth factor, detection/invisibility/reveal resistance, noise/light scalars). Replicated; clamps resistances/scalars to 0..1 and caps StealthFactor. For use by detection/visibility logic (not applied in damage).
UCLASS(BlueprintType)
class POLYCHARACTERSRUNTIME_API UPolyStealthSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:
	UPolyStealthSet();

	ATTRIBUTE_ACCESSORS(UPolyStealthSet, StealthFactor);
	ATTRIBUTE_ACCESSORS(UPolyStealthSet, DetectionResistance);
	ATTRIBUTE_ACCESSORS(UPolyStealthSet, InvisibilityStrength);
	ATTRIBUTE_ACCESSORS(UPolyStealthSet, RevealResistance);
	ATTRIBUTE_ACCESSORS(UPolyStealthSet, NoiseScalar);
	ATTRIBUTE_ACCESSORS(UPolyStealthSet, LightVisibilityScalar);

protected:
	UFUNCTION() void OnRep_StealthFactor(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_DetectionResistance(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_InvisibilityStrength(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_RevealResistance(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_NoiseScalar(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_LightVisibilityScalar(const FGameplayAttributeData& OldValue);

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StealthFactor, Category="Poly|Stealth", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData StealthFactor;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_DetectionResistance, Category="Poly|Stealth", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData DetectionResistance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_InvisibilityStrength, Category="Poly|Stealth", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData InvisibilityStrength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_RevealResistance, Category="Poly|Stealth", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData RevealResistance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_NoiseScalar, Category="Poly|Stealth", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData NoiseScalar;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_LightVisibilityScalar, Category="Poly|Stealth", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData LightVisibilityScalar;
};


