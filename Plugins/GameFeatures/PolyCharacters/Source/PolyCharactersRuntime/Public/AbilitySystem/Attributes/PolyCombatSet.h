// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "AbilitySystemComponent.h"

#include "PolyCombatSet.generated.h"

// Offense/mitigation/utility/regeneration attributes (power, crit, block, thorns, lifesteal, regen, movement scalar). Replicated; clamps chances/fractions to 0..1, crit mult >=1, movement >0, regen/mitigation >=0. Used by PolyDamageExecution for combat math.
UCLASS(BlueprintType)
class POLYCHARACTERSRUNTIME_API UPolyCombatSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:
	UPolyCombatSet();

	// Offense
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, DamagePower);      // scales outgoing damage
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, CritChance);       // 0..1
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, CritMultiplier);   // >= 1

	// Mitigation / reactive
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, FlatMitigation);   // flat pre-resistance reduction
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, BlockChance);      // 0..1
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, BlockAmount);      // 0..1 fraction reduced on block
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, Thorns);           // 0..1 fraction reflected to instigator

	// Sustain
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, LifeSteal);        // 0..1 fraction of final damage healed
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, ShieldGainOnHit);  // 0..1 fraction of final damage gained as shield

	// Utility / CC
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, Tenacity);         // 0..1 reduces CC duration
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, MoveSpeedScalar);  // >0 movement scalar

	// Regeneration
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, HealthRegen);      // per-second or per-tick, game-defined
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, StaminaRegen);     // per-second or per-tick, game-defined
	ATTRIBUTE_ACCESSORS(UPolyCombatSet, ShieldRegenRate);  // per-second or per-tick, game-defined

protected:
	UFUNCTION()
	void OnRep_DamagePower(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_CritChance(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_CritMultiplier(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_FlatMitigation(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_BlockAmount(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Thorns(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_LifeSteal(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_ShieldGainOnHit(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_Tenacity(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MoveSpeedScalar(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_StaminaRegen(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_ShieldRegenRate(const FGameplayAttributeData& OldValue);

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_DamagePower,   Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData DamagePower;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CritChance,    Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CritChance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CritMultiplier,Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData CritMultiplier;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FlatMitigation,Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData FlatMitigation;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BlockChance,   Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData BlockChance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BlockAmount,   Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData BlockAmount;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Thorns,        Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Thorns;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_LifeSteal,     Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData LifeSteal;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ShieldGainOnHit,Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData ShieldGainOnHit;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Tenacity,      Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Tenacity;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MoveSpeedScalar,Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MoveSpeedScalar;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HealthRegen,   Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData HealthRegen;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StaminaRegen,  Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData StaminaRegen;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ShieldRegenRate,Category="Poly|Combat", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData ShieldRegenRate;
};


