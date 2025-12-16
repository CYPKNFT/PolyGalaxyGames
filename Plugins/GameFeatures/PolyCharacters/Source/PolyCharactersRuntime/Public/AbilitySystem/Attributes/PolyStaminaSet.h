// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"

#include "PolyStaminaSet.generated.h"

class UObject;
struct FFrame;
struct FGameplayEffectModCallbackData;

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Poly_StaminaImmunity);

namespace PolyGameplayTags
{
	POLYCHARACTERSRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedStamina);
}

/**
 * Stamina attribute set.
 */
UCLASS(BlueprintType)
class POLYCHARACTERSRUNTIME_API UPolyStaminaSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:
	UPolyStaminaSet();

	// Stamina pool, cap, and meta incoming gain (Recuperating cleared after use)
	ATTRIBUTE_ACCESSORS(UPolyStaminaSet, Stamina);
	ATTRIBUTE_ACCESSORS(UPolyStaminaSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UPolyStaminaSet, Recuperating);
	// Sprinting: absolute move speed while sprinting and costs
	ATTRIBUTE_ACCESSORS(UPolyStaminaSet, MoveSpeed);
	ATTRIBUTE_ACCESSORS(UPolyStaminaSet, SprintStaminaCostPerSecond);

	mutable FLyraAttributeEvent OnStaminaChanged;
	mutable FLyraAttributeEvent OnMaxStaminaChanged;
	mutable FLyraAttributeEvent OnOutOfStamina;

protected:
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_SprintStaminaCostPerSecond(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Stamina, Category="Poly|Stamina", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxStamina, Category="Poly|Stamina", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxStamina;

	UPROPERTY(BlueprintReadOnly, Category="Poly|Stamina", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Recuperating;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MoveSpeed, Category="Poly|Stamina|Sprint", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MoveSpeed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_SprintStaminaCostPerSecond, Category="Poly|Stamina|Sprint", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData SprintStaminaCostPerSecond;

	bool bOutOfStamina;
	float MaxStaminaBeforeAttributeChange;
	float StaminaBeforeAttributeChange;
};

