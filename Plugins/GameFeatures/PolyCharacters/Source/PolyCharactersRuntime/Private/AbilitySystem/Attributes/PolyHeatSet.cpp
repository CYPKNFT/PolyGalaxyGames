// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Attributes/PolyHeatSet.h"

#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PolyHeatSet)

namespace PolyHeatClamps
{
	static constexpr float MinZero = 0.0f;
	static constexpr float MaxWanted = 7.0f;
}

UPolyHeatSet::UPolyHeatSet()
	: WantedLevel(0.0f)
	, HeatDecayRate(0.0f)
	, CrimeNotoriety(0.0f)
	, BribeCostScalar(1.0f)
{
}

void UPolyHeatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPolyHeatSet, WantedLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyHeatSet, HeatDecayRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyHeatSet, CrimeNotoriety, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyHeatSet, BribeCostScalar, COND_None, REPNOTIFY_Always);
}

void UPolyHeatSet::OnRep_WantedLevel(const FGameplayAttributeData& OldValue)    { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyHeatSet, WantedLevel, OldValue); }
void UPolyHeatSet::OnRep_HeatDecayRate(const FGameplayAttributeData& OldValue)  { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyHeatSet, HeatDecayRate, OldValue); }
void UPolyHeatSet::OnRep_CrimeNotoriety(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyHeatSet, CrimeNotoriety, OldValue); }
void UPolyHeatSet::OnRep_BribeCostScalar(const FGameplayAttributeData& OldValue){ GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyHeatSet, BribeCostScalar, OldValue); }

void UPolyHeatSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPolyHeatSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPolyHeatSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	using namespace PolyHeatClamps;

	if (Attribute == GetWantedLevelAttribute())
	{
		NewValue = FMath::Clamp(NewValue, MinZero, MaxWanted);
	}
	else if (Attribute == GetHeatDecayRateAttribute()
		|| Attribute == GetCrimeNotorietyAttribute()
		|| Attribute == GetBribeCostScalarAttribute())
	{
		NewValue = FMath::Max(NewValue, MinZero);
	}
}



