// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "GameplayEffectTypes.h"

#include "PolyCombatComponent.generated.h"

struct FOnAttributeChangeData;
class ULyraAbilitySystemComponent;
class UPolyCombatSet;

// Fired when any combat attribute changes; Attribute identifies which one.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FCombatAttributeChanged, UPolyCombatComponent*, CombatComponent, FGameplayAttribute, Attribute, float, OldValue, float, NewValue);

/**
 * Thin component to surface PolyCombatSet attributes to Blueprint/UI/AI.
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class POLYCHARACTERSRUNTIME_API UPolyCombatComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UPolyCombatComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category="Poly|Combat")
	static UPolyCombatComponent* FindCombatComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UPolyCombatComponent>() : nullptr); }

	UFUNCTION(BlueprintCallable, Category="Poly|Combat")
	void InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category="Poly|Combat")
	void UninitializeFromAbilitySystem();

	// Getters (no normalization needed)
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetDamagePower() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetCritChance() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetCritMultiplier() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetFlatMitigation() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetBlockChance() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetBlockAmount() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetThorns() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetLifeSteal() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetShieldGainOnHit() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetTenacity() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetMoveSpeedScalar() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetHealthRegen() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetStaminaRegen() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Combat") float GetShieldRegenRate() const;

public:
	UPROPERTY(BlueprintAssignable)
	FCombatAttributeChanged OnCombatAttributeChanged;

protected:
	virtual void OnUnregister() override;

	void BindAttributeDelegates();
	void HandleAttributeChanged(const FOnAttributeChangeData& ChangeData, const FGameplayAttribute& Attribute);

	UPROPERTY()
	TObjectPtr<ULyraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const UPolyCombatSet> CombatSet;
};


