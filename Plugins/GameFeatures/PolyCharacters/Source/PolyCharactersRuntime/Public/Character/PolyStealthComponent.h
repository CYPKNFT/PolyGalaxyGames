// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "GameplayEffectTypes.h"

#include "PolyStealthComponent.generated.h"

struct FOnAttributeChangeData;
class ULyraAbilitySystemComponent;
class UPolyStealthSet;

// Fired when any stealth attribute changes; Attribute identifies which one.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FStealthAttributeChanged, UPolyStealthComponent*, StealthComponent, FGameplayAttribute, Attribute, float, OldValue, float, NewValue);

// Component that exposes stealth-related attributes (PolyStealthSet) to Blueprint/UI/AI and raises change events.
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class POLYCHARACTERSRUNTIME_API UPolyStealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UPolyStealthComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category="Poly|Stealth")
	static UPolyStealthComponent* FindStealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UPolyStealthComponent>() : nullptr); }

	// Must be called after the owning actor's ASC exists.
	UFUNCTION(BlueprintCallable, Category="Poly|Stealth")
	void InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category="Poly|Stealth")
	void UninitializeFromAbilitySystem();

	// Accessors
	UFUNCTION(BlueprintCallable, Category="Poly|Stealth") float GetStealthFactor() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Stealth") float GetDetectionResistance() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Stealth") float GetInvisibilityStrength() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Stealth") float GetRevealResistance() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Stealth") float GetNoiseScalar() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Stealth") float GetLightVisibilityScalar() const;

public:
	UPROPERTY(BlueprintAssignable)
	FStealthAttributeChanged OnStealthAttributeChanged;

protected:
	virtual void OnUnregister() override;

	void BindAttributeDelegates();
	void HandleAttributeChanged(const FOnAttributeChangeData& ChangeData, const FGameplayAttribute& Attribute);

	UPROPERTY()
	TObjectPtr<ULyraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const UPolyStealthSet> StealthSet;
};




