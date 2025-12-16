// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/PolyStaminaComponent.h"

#include "AbilitySystem/Attributes/PolyStaminaSet.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "LyraLogChannels.h"

UPolyStaminaComponent::UPolyStaminaComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	StaminaSet = nullptr;
}

void UPolyStaminaComponent::InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyStaminaComponent: Already initialized for owner [%s]."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyStaminaComponent: Cannot initialize for owner [%s] with NULL ASC."), *GetNameSafe(Owner));
		return;
	}

	StaminaSet = AbilitySystemComponent->GetSet<UPolyStaminaSet>();
	if (!StaminaSet)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyStaminaComponent: Missing PolyStaminaSet on ASC for owner [%s]."), *GetNameSafe(Owner));
		return;
	}

	// Bind change delegates for Blueprint/UI consumption.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyStaminaSet::GetStaminaAttribute()).AddUObject(this, &ThisClass::HandleStaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyStaminaSet::GetMaxStaminaAttribute()).AddUObject(this, &ThisClass::HandleMaxStaminaChanged);

	OnStaminaChanged.Broadcast(this, StaminaSet->GetMaxStamina(), StaminaSet->GetStamina());
}

void UPolyStaminaComponent::UninitializeFromAbilitySystem()
{
	StaminaSet = nullptr;
	AbilitySystemComponent = nullptr;
}

float UPolyStaminaComponent::GetStamina() const
{
	return (StaminaSet ? StaminaSet->GetStamina() : 0.0f);
}

float UPolyStaminaComponent::GetMaxStamina() const
{
	return (StaminaSet ? StaminaSet->GetMaxStamina() : 0.0f);
}

float UPolyStaminaComponent::GetStaminaNormalized() const
{
	if (StaminaSet)
	{
		const float CurrentStamina = StaminaSet->GetStamina();
		const float Max = StaminaSet->GetMaxStamina();
		return (Max > 0.0f) ? (CurrentStamina / Max) : 0.0f;
	}

	return 0.0f;
}

void UPolyStaminaComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	Super::OnUnregister();
}

void UPolyStaminaComponent::HandleStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	OnStaminaChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue);
}

void UPolyStaminaComponent::HandleMaxStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	OnMaxStaminaChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue);
}

