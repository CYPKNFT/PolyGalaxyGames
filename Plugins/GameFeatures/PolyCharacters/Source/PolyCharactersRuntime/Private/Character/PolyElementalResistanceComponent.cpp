// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/PolyElementalResistanceComponent.h"

#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/PolyElementalResistanceSet.h"
#include "PolyGameplayTags.h"
#include "LyraLogChannels.h"

UPolyElementalResistanceComponent::UPolyElementalResistanceComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	ResistanceSet = nullptr;
}

void UPolyElementalResistanceComponent::InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyElementalResistanceComponent: Already initialized for owner [%s]."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyElementalResistanceComponent: Cannot initialize for owner [%s] with NULL ASC."), *GetNameSafe(Owner));
		return;
	}

	ResistanceSet = AbilitySystemComponent->GetSet<UPolyElementalResistanceSet>();
	if (!ResistanceSet)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyElementalResistanceComponent: Missing PolyElementalResistanceSet on ASC for owner [%s]."), *GetNameSafe(Owner));
		return;
	}

	// Bind change delegates per resistance attribute to broadcast Blueprint-friendly events.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyElementalResistanceSet::GetResistRadiationAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleResistanceChanged(Data, UPolyElementalResistanceSet::GetResistRadiationAttribute(), TAG_Poly_Damage_Radiation); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyElementalResistanceSet::GetResistIncendiaryAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleResistanceChanged(Data, UPolyElementalResistanceSet::GetResistIncendiaryAttribute(), TAG_Poly_Damage_Incendiary); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyElementalResistanceSet::GetResistShockAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleResistanceChanged(Data, UPolyElementalResistanceSet::GetResistShockAttribute(), TAG_Poly_Damage_Shock); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyElementalResistanceSet::GetResistFreezeAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleResistanceChanged(Data, UPolyElementalResistanceSet::GetResistFreezeAttribute(), TAG_Poly_Damage_Freeze); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyElementalResistanceSet::GetResistPoisonAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleResistanceChanged(Data, UPolyElementalResistanceSet::GetResistPoisonAttribute(), TAG_Poly_Damage_Poison); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyElementalResistanceSet::GetResistNaniteAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleResistanceChanged(Data, UPolyElementalResistanceSet::GetResistNaniteAttribute(), TAG_Poly_Damage_Nanite); });
}

void UPolyElementalResistanceComponent::UninitializeFromAbilitySystem()
{
	ResistanceSet = nullptr;
	AbilitySystemComponent = nullptr;
}

float UPolyElementalResistanceComponent::GetResistRadiation() const
{
	return ResistanceSet ? ResistanceSet->GetResistRadiation() : 0.0f;
}

float UPolyElementalResistanceComponent::GetResistIncendiary() const
{
	return ResistanceSet ? ResistanceSet->GetResistIncendiary() : 0.0f;
}

float UPolyElementalResistanceComponent::GetResistShock() const
{
	return ResistanceSet ? ResistanceSet->GetResistShock() : 0.0f;
}

float UPolyElementalResistanceComponent::GetResistFreeze() const
{
	return ResistanceSet ? ResistanceSet->GetResistFreeze() : 0.0f;
}

float UPolyElementalResistanceComponent::GetResistPoison() const
{
	return ResistanceSet ? ResistanceSet->GetResistPoison() : 0.0f;
}

float UPolyElementalResistanceComponent::GetResistNanite() const
{
	return ResistanceSet ? ResistanceSet->GetResistNanite() : 0.0f;
}

void UPolyElementalResistanceComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	Super::OnUnregister();
}

void UPolyElementalResistanceComponent::HandleResistanceChanged(const FOnAttributeChangeData& ChangeData, const FGameplayAttribute& Attribute, const FGameplayTag& ResistanceTag)
{
	OnResistanceChanged.Broadcast(this, ResistanceTag, ChangeData.OldValue, ChangeData.NewValue);
}

