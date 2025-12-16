// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/PolyCombatComponent.h"

#include "AbilitySystem/Attributes/PolyCombatSet.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "LyraLogChannels.h"

UPolyCombatComponent::UPolyCombatComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	CombatSet = nullptr;
}

void UPolyCombatComponent::InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyCombatComponent: Already initialized for owner [%s]."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyCombatComponent: Cannot initialize for owner [%s] with NULL ASC."), *GetNameSafe(Owner));
		return;
	}

	CombatSet = AbilitySystemComponent->GetSet<UPolyCombatSet>();
	if (!CombatSet)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyCombatComponent: Missing PolyCombatSet on ASC for owner [%s]."), *GetNameSafe(Owner));
		return;
	}

	BindAttributeDelegates();
}

void UPolyCombatComponent::UninitializeFromAbilitySystem()
{
	CombatSet = nullptr;
	AbilitySystemComponent = nullptr;
}

float UPolyCombatComponent::GetDamagePower() const        { return CombatSet ? CombatSet->GetDamagePower() : 0.0f; }
float UPolyCombatComponent::GetCritChance() const         { return CombatSet ? CombatSet->GetCritChance() : 0.0f; }
float UPolyCombatComponent::GetCritMultiplier() const     { return CombatSet ? CombatSet->GetCritMultiplier() : 1.0f; }
float UPolyCombatComponent::GetFlatMitigation() const     { return CombatSet ? CombatSet->GetFlatMitigation() : 0.0f; }
float UPolyCombatComponent::GetBlockChance() const        { return CombatSet ? CombatSet->GetBlockChance() : 0.0f; }
float UPolyCombatComponent::GetBlockAmount() const        { return CombatSet ? CombatSet->GetBlockAmount() : 0.0f; }
float UPolyCombatComponent::GetThorns() const             { return CombatSet ? CombatSet->GetThorns() : 0.0f; }
float UPolyCombatComponent::GetLifeSteal() const          { return CombatSet ? CombatSet->GetLifeSteal() : 0.0f; }
float UPolyCombatComponent::GetShieldGainOnHit() const    { return CombatSet ? CombatSet->GetShieldGainOnHit() : 0.0f; }
float UPolyCombatComponent::GetTenacity() const           { return CombatSet ? CombatSet->GetTenacity() : 0.0f; }
float UPolyCombatComponent::GetMoveSpeedScalar() const    { return CombatSet ? CombatSet->GetMoveSpeedScalar() : 1.0f; }
float UPolyCombatComponent::GetHealthRegen() const        { return CombatSet ? CombatSet->GetHealthRegen() : 0.0f; }
float UPolyCombatComponent::GetStaminaRegen() const       { return CombatSet ? CombatSet->GetStaminaRegen() : 0.0f; }
float UPolyCombatComponent::GetShieldRegenRate() const    { return CombatSet ? CombatSet->GetShieldRegenRate() : 0.0f; }

void UPolyCombatComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	Super::OnUnregister();
}

void UPolyCombatComponent::BindAttributeDelegates()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	const TArray<FGameplayAttribute> Attrs =
	{
		UPolyCombatSet::GetDamagePowerAttribute(),
		UPolyCombatSet::GetCritChanceAttribute(),
		UPolyCombatSet::GetCritMultiplierAttribute(),
		UPolyCombatSet::GetFlatMitigationAttribute(),
		UPolyCombatSet::GetBlockChanceAttribute(),
		UPolyCombatSet::GetBlockAmountAttribute(),
		UPolyCombatSet::GetThornsAttribute(),
		UPolyCombatSet::GetLifeStealAttribute(),
		UPolyCombatSet::GetShieldGainOnHitAttribute(),
		UPolyCombatSet::GetTenacityAttribute(),
		UPolyCombatSet::GetMoveSpeedScalarAttribute(),
		UPolyCombatSet::GetHealthRegenAttribute(),
		UPolyCombatSet::GetStaminaRegenAttribute(),
		UPolyCombatSet::GetShieldRegenRateAttribute(),
	};

	for (const FGameplayAttribute& Attr : Attrs)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attr).AddLambda(
			[this, Attr](const FOnAttributeChangeData& Data)
			{
				HandleAttributeChanged(Data, Attr);
			});
	}
}

void UPolyCombatComponent::HandleAttributeChanged(const FOnAttributeChangeData& ChangeData, const FGameplayAttribute& Attribute)
{
	OnCombatAttributeChanged.Broadcast(this, Attribute, ChangeData.OldValue, ChangeData.NewValue);
}



