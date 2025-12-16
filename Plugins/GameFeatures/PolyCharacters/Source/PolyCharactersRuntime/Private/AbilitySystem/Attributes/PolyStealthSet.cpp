// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Attributes/PolyStealthSet.h"

#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PolyStealthSet)

namespace PolyStealthClamps
{
	static constexpr float MinZero = 0.0f;
	static constexpr float MaxOne  = 1.0f;
	static constexpr float StealthFactorMax = 2.0f; // allow mild buffs >1 while bounded
}

UPolyStealthSet::UPolyStealthSet()
	: StealthFactor(1.0f)
	, DetectionResistance(0.0f)
	, InvisibilityStrength(0.0f)
	, RevealResistance(0.0f)
	, NoiseScalar(1.0f)
	, LightVisibilityScalar(1.0f)
{
}

void UPolyStealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPolyStealthSet, StealthFactor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyStealthSet, DetectionResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyStealthSet, InvisibilityStrength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyStealthSet, RevealResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyStealthSet, NoiseScalar, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyStealthSet, LightVisibilityScalar, COND_None, REPNOTIFY_Always);
}

void UPolyStealthSet::OnRep_StealthFactor(const FGameplayAttributeData& OldValue)        { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyStealthSet, StealthFactor, OldValue); }
void UPolyStealthSet::OnRep_DetectionResistance(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyStealthSet, DetectionResistance, OldValue); }
void UPolyStealthSet::OnRep_InvisibilityStrength(const FGameplayAttributeData& OldValue){ GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyStealthSet, InvisibilityStrength, OldValue); }
void UPolyStealthSet::OnRep_RevealResistance(const FGameplayAttributeData& OldValue)    { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyStealthSet, RevealResistance, OldValue); }
void UPolyStealthSet::OnRep_NoiseScalar(const FGameplayAttributeData& OldValue)         { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyStealthSet, NoiseScalar, OldValue); }
void UPolyStealthSet::OnRep_LightVisibilityScalar(const FGameplayAttributeData& OldValue){ GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyStealthSet, LightVisibilityScalar, OldValue); }

void UPolyStealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPolyStealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPolyStealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	using namespace PolyStealthClamps;

	if (Attribute == GetStealthFactorAttribute())
	{
		NewValue = FMath::Clamp(NewValue, MinZero, StealthFactorMax);
	}
	else if (Attribute == GetDetectionResistanceAttribute()
		|| Attribute == GetInvisibilityStrengthAttribute()
		|| Attribute == GetRevealResistanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, MinZero, MaxOne);
	}
	else if (Attribute == GetNoiseScalarAttribute()
		|| Attribute == GetLightVisibilityScalarAttribute())
	{
		NewValue = FMath::Clamp(NewValue, MinZero, MaxOne);
	}
}



