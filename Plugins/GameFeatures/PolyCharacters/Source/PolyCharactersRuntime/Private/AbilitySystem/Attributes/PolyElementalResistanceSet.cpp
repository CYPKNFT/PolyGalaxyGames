// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Attributes/PolyElementalResistanceSet.h"

#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PolyElementalResistanceSet)

UPolyElementalResistanceSet::UPolyElementalResistanceSet()
	: ResistRadiation(0.f)
	, ResistIncendiary(0.f)
	, ResistShock(0.f)
	, ResistFreeze(0.f)
	, ResistPoison(0.f)
	, ResistNanite(0.f)
{
}

void UPolyElementalResistanceSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Always replicate resistances so mitigation is correct on clients.
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyElementalResistanceSet, ResistRadiation, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyElementalResistanceSet, ResistIncendiary, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyElementalResistanceSet, ResistShock, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyElementalResistanceSet, ResistFreeze, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyElementalResistanceSet, ResistPoison, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyElementalResistanceSet, ResistNanite, COND_None, REPNOTIFY_Always);
}

void UPolyElementalResistanceSet::OnRep_ResistRadiation(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyElementalResistanceSet, ResistRadiation, OldValue);
}

void UPolyElementalResistanceSet::OnRep_ResistIncendiary(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyElementalResistanceSet, ResistIncendiary, OldValue);
}

void UPolyElementalResistanceSet::OnRep_ResistShock(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyElementalResistanceSet, ResistShock, OldValue);
}

void UPolyElementalResistanceSet::OnRep_ResistFreeze(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyElementalResistanceSet, ResistFreeze, OldValue);
}

void UPolyElementalResistanceSet::OnRep_ResistPoison(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyElementalResistanceSet, ResistPoison, OldValue);
}

void UPolyElementalResistanceSet::OnRep_ResistNanite(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyElementalResistanceSet, ResistNanite, OldValue);
}

void UPolyElementalResistanceSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPolyElementalResistanceSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPolyElementalResistanceSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	static constexpr float MinResist = 0.0f;
	static constexpr float MaxResist = 1.0f;

	if (Attribute == GetResistRadiationAttribute()
		|| Attribute == GetResistIncendiaryAttribute()
		|| Attribute == GetResistShockAttribute()
		|| Attribute == GetResistFreezeAttribute()
		|| Attribute == GetResistPoisonAttribute()
		|| Attribute == GetResistNaniteAttribute())
	{
		// Keep resistances in [0..1] to keep the simple (1 - resistance) math valid.
		NewValue = FMath::Clamp(NewValue, MinResist, MaxResist);
	}
}

