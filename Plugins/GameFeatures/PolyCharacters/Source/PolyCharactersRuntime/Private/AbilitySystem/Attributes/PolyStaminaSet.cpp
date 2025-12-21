// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Attributes/PolyStaminaSet.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "LyraGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PolyStaminaSet)

UE_DEFINE_GAMEPLAY_TAG(TAG_Poly_StaminaImmunity, "Gameplay.Poly.StaminaImmunity");

namespace PolyGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_UnlimitedStamina, "Cheat.Poly.UnlimitedStamina", "Unlimited stamina cheat on owner.");
}

UPolyStaminaSet::UPolyStaminaSet()
	: Stamina(100.0f)
	, MaxStamina(100.0f)
	, Recuperating(0.0f)
	, MoveSpeed(600.0f) // absolute sprint speed (walk is 600)
	, SprintStaminaCostPerSecond(15.0f)
{
	bOutOfStamina = false;
	MaxStaminaBeforeAttributeChange = 0.0f;
	StaminaBeforeAttributeChange = 0.0f;
}

void UPolyStaminaSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate stamina state so HUD/clients stay in sync.
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyStaminaSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyStaminaSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyStaminaSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPolyStaminaSet, SprintStaminaCostPerSecond, COND_None, REPNOTIFY_Always);
}

void UPolyStaminaSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyStaminaSet, Stamina, OldValue);

	const float CurrentStamina = GetStamina();
	const float EstimatedMagnitude = CurrentStamina - OldValue.GetCurrentValue();

	OnStaminaChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentStamina);

	if (!bOutOfStamina && CurrentStamina <= 0.0f)
	{
		OnOutOfStamina.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentStamina);
	}

	bOutOfStamina = (CurrentStamina <= 0.0f);
}

void UPolyStaminaSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyStaminaSet, MaxStamina, OldValue);
	OnMaxStaminaChanged.Broadcast(nullptr, nullptr, nullptr, GetMaxStamina() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetMaxStamina());
}

void UPolyStaminaSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyStaminaSet, MoveSpeed, OldValue);
}

void UPolyStaminaSet::OnRep_SprintStaminaCostPerSecond(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPolyStaminaSet, SprintStaminaCostPerSecond, OldValue);
}

bool UPolyStaminaSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	// Cache for callbacks and clamping after the effect is applied.
	StaminaBeforeAttributeChange = GetStamina();
	MaxStaminaBeforeAttributeChange = GetMaxStamina();

	return true;
}

void UPolyStaminaSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumStamina = 0.0f;

#if !UE_BUILD_SHIPPING
	if (Data.Target.HasMatchingGameplayTag(LyraGameplayTags::Cheat_GodMode) || Data.Target.HasMatchingGameplayTag(PolyGameplayTags::Cheat_UnlimitedStamina))
	{
		MinimumStamina = 1.0f;
	}
#endif

	if (Data.EvaluatedData.Attribute == GetRecuperatingAttribute())
	{
		// Convert meta into real stamina, then clear it.
		SetStamina(FMath::Clamp(GetStamina() + GetRecuperating(), MinimumStamina, GetMaxStamina()));
		SetRecuperating(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), MinimumStamina, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
		OnMaxStaminaChanged.Broadcast(Data.EffectSpec.GetEffectContext().GetOriginalInstigator(), Data.EffectSpec.GetEffectContext().GetEffectCauser(), &Data.EffectSpec, Data.EvaluatedData.Magnitude, MaxStaminaBeforeAttributeChange, GetMaxStamina());
	}
	else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		// Clamp sprint move speed after effects apply.
		SetMoveSpeed(FMath::Clamp(GetMoveSpeed(), 0.0f, 1200.0f));
	}

	if (GetStamina() != StaminaBeforeAttributeChange)
	{
		OnStaminaChanged.Broadcast(Data.EffectSpec.GetEffectContext().GetOriginalInstigator(), Data.EffectSpec.GetEffectContext().GetEffectCauser(), &Data.EffectSpec, Data.EvaluatedData.Magnitude, StaminaBeforeAttributeChange, GetStamina());
	}

	if ((GetStamina() <= 0.0f) && !bOutOfStamina)
	{
		OnOutOfStamina.Broadcast(Data.EffectSpec.GetEffectContext().GetOriginalInstigator(), Data.EffectSpec.GetEffectContext().GetEffectCauser(), &Data.EffectSpec, Data.EvaluatedData.Magnitude, StaminaBeforeAttributeChange, GetStamina());
	}

	bOutOfStamina = (GetStamina() <= 0.0f);
}

void UPolyStaminaSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPolyStaminaSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UPolyStaminaSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxStaminaAttribute())
	{
		if (GetStamina() > NewValue)
		{
			ULyraAbilitySystemComponent* LyraASC = GetLyraAbilitySystemComponent();
			check(LyraASC);

			LyraASC->ApplyModToAttribute(GetStaminaAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (bOutOfStamina && (GetStamina() > 0.0f))
	{
		bOutOfStamina = false;
	}
}

void UPolyStaminaSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetStaminaAttribute())
	{
		// Prevent stamina from going below 0 or above max.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		// Keep a sane positive max stamina.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		// Prevent invalid speeds; clamp to a reasonable sprint range.
		NewValue = FMath::Clamp(NewValue, 0.0f, 3000.0f);
	}
	else if (Attribute == GetSprintStaminaCostPerSecondAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

