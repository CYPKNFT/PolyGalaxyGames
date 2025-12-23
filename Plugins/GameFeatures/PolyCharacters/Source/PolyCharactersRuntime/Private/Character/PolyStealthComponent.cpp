// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/PolyStealthComponent.h"

#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/PolyStealthSet.h"
#include "LyraLogChannels.h"

UPolyStealthComponent::UPolyStealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	StealthSet = nullptr;
}

void UPolyStealthComponent::InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyStealthComponent: Already initialized for owner [%s]."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyStealthComponent: Cannot initialize for owner [%s] with NULL ASC."), *GetNameSafe(Owner));
		return;
	}

	StealthSet = AbilitySystemComponent->GetSet<UPolyStealthSet>();
	if (!StealthSet)
	{
		UE_LOG(LogLyra, Error, TEXT("PolyStealthComponent: Missing PolyStealthSet on ASC for owner [%s]."), *GetNameSafe(Owner));
		return;
	}

	BindAttributeDelegates();
}

void UPolyStealthComponent::UninitializeFromAbilitySystem()
{
	StealthSet = nullptr;
	AbilitySystemComponent = nullptr;
}

float UPolyStealthComponent::GetStealthFactor() const
{
	return StealthSet ? StealthSet->GetStealthFactor() : 1.0f;
}

float UPolyStealthComponent::GetDetectionResistance() const
{
	return StealthSet ? StealthSet->GetDetectionResistance() : 0.0f;
}

float UPolyStealthComponent::GetInvisibilityStrength() const
{
	return StealthSet ? StealthSet->GetInvisibilityStrength() : 0.0f;
}

float UPolyStealthComponent::GetRevealResistance() const
{
	return StealthSet ? StealthSet->GetRevealResistance() : 0.0f;
}

float UPolyStealthComponent::GetNoiseScalar() const
{
	return StealthSet ? StealthSet->GetNoiseScalar() : 0.0f;
}

float UPolyStealthComponent::GetLightVisibilityScalar() const
{
	return StealthSet ? StealthSet->GetLightVisibilityScalar() : 0.0f;
}

void UPolyStealthComponent::OnUnregister()
{
	UninitializeFromAbilitySystem();
	Super::OnUnregister();
}

void UPolyStealthComponent::BindAttributeDelegates()
{
	check(AbilitySystemComponent);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyStealthSet::GetStealthFactorAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleAttributeChanged(Data, UPolyStealthSet::GetStealthFactorAttribute()); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyStealthSet::GetDetectionResistanceAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleAttributeChanged(Data, UPolyStealthSet::GetDetectionResistanceAttribute()); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyStealthSet::GetInvisibilityStrengthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleAttributeChanged(Data, UPolyStealthSet::GetInvisibilityStrengthAttribute()); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyStealthSet::GetRevealResistanceAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleAttributeChanged(Data, UPolyStealthSet::GetRevealResistanceAttribute()); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyStealthSet::GetNoiseScalarAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleAttributeChanged(Data, UPolyStealthSet::GetNoiseScalarAttribute()); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPolyStealthSet::GetLightVisibilityScalarAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ HandleAttributeChanged(Data, UPolyStealthSet::GetLightVisibilityScalarAttribute()); });
}

void UPolyStealthComponent::HandleAttributeChanged(const FOnAttributeChangeData& ChangeData, const FGameplayAttribute& Attribute)
{
	OnStealthAttributeChanged.Broadcast(this, Attribute, ChangeData.OldValue, ChangeData.NewValue);
}




