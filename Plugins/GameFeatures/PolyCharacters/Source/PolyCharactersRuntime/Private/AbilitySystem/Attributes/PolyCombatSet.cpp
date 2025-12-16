// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Attributes/PolyCombatSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PolyCombatSet)

namespace PolyCombatSetClamps
{
	static constexpr float MinZero = 0.0f;
	static constexpr float MaxOne  = 1.0f;
	static constexpr float MinCritMultiplier = 1.0f;
	static constexpr float MinMoveSpeedScalar = 0.01f; // avoid zero/negative
}

UPolyCombatSet::UPolyCombatSet()
	: DamagePower(1.0f)
	, CritChance(0.0f)
	, CritMultiplier(1.5f)
	, FlatMitigation(0.0f)
	, BlockChance(0.0f)
	, BlockAmount(0.5f)
	, Thorns(0.0f)
	, LifeSteal(0.0f)
	, ShieldGainOnHit(0.0f)
	, Tenacity(0.0f)
	, MoveSpeedScalar(1.0f)
	, HealthRegen(0.0f)
	, StaminaRegen(0.0f)
	, ShieldRegenRate(0.0f)
{
}

void UPolyCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, DamagePower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, CritMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, FlatMitigation, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, BlockAmount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, Thorns, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, LifeSteal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, ShieldGainOnHit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, Tenacity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, MoveSpeedScalar, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, StaminaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyCombatSet, ShieldRegenRate, COND_None, REPNOTIFY_Always);
}

void UPolyCombatSet::OnRep_DamagePower(const FGameplayAttributeData& OldValue)        { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, DamagePower, OldValue); }
void UPolyCombatSet::OnRep_CritChance(const FGameplayAttributeData& OldValue)         { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, CritChance, OldValue); }
void UPolyCombatSet::OnRep_CritMultiplier(const FGameplayAttributeData& OldValue)     { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, CritMultiplier, OldValue); }
void UPolyCombatSet::OnRep_FlatMitigation(const FGameplayAttributeData& OldValue)     { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, FlatMitigation, OldValue); }
void UPolyCombatSet::OnRep_BlockChance(const FGameplayAttributeData& OldValue)        { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, BlockChance, OldValue); }
void UPolyCombatSet::OnRep_BlockAmount(const FGameplayAttributeData& OldValue)        { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, BlockAmount, OldValue); }
void UPolyCombatSet::OnRep_Thorns(const FGameplayAttributeData& OldValue)             { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, Thorns, OldValue); }
void UPolyCombatSet::OnRep_LifeSteal(const FGameplayAttributeData& OldValue)          { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, LifeSteal, OldValue); }
void UPolyCombatSet::OnRep_ShieldGainOnHit(const FGameplayAttributeData& OldValue)    { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, ShieldGainOnHit, OldValue); }
void UPolyCombatSet::OnRep_Tenacity(const FGameplayAttributeData& OldValue)           { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, Tenacity, OldValue); }
void UPolyCombatSet::OnRep_MoveSpeedScalar(const FGameplayAttributeData& OldValue)    { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, MoveSpeedScalar, OldValue); }
void UPolyCombatSet::OnRep_HealthRegen(const FGameplayAttributeData& OldValue)        { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, HealthRegen, OldValue); }
void UPolyCombatSet::OnRep_StaminaRegen(const FGameplayAttributeData& OldValue)       { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, StaminaRegen, OldValue); }
void UPolyCombatSet::OnRep_ShieldRegenRate(const FGameplayAttributeData& OldValue)    { GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyCombatSet, ShieldRegenRate, OldValue); }

void UPolyCombatSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPolyCombatSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPolyCombatSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	using namespace PolyCombatSetClamps;

	if (Attribute == GetCritChanceAttribute()
		|| Attribute == GetBlockChanceAttribute()
		|| Attribute == GetBlockAmountAttribute()
		|| Attribute == GetThornsAttribute()
		|| Attribute == GetLifeStealAttribute()
		|| Attribute == GetShieldGainOnHitAttribute()
		|| Attribute == GetTenacityAttribute())
	{
		NewValue = FMath::Clamp(NewValue, MinZero, MaxOne);
	}
	else if (Attribute == GetCritMultiplierAttribute())
	{
		NewValue = FMath::Max(NewValue, MinCritMultiplier);
	}
	else if (Attribute == GetMoveSpeedScalarAttribute())
	{
		NewValue = FMath::Max(NewValue, MinMoveSpeedScalar);
	}
	else if (Attribute == GetDamagePowerAttribute()
		|| Attribute == GetFlatMitigationAttribute()
		|| Attribute == GetHealthRegenAttribute()
		|| Attribute == GetStaminaRegenAttribute()
		|| Attribute == GetShieldRegenRateAttribute())
	{
		NewValue = FMath::Max(NewValue, MinZero);
	}
}


