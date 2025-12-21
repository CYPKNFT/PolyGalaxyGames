// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"

#include "PolyElementalResistanceComponent.generated.h"

struct FOnAttributeChangeData;
class ULyraAbilitySystemComponent;
class UPolyElementalResistanceSet;

// Fired whenever a resistance attribute changes (e.g., buff/debuff).
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FResistance_AttributeChanged, UPolyElementalResistanceComponent*, ResistanceComponent, FGameplayTag, ResistanceTag, float, OldValue, float, NewValue);

// Component exposing per-damage-type resistances and change delegates (init with ASC). Reads PolyElementalResistanceSet and broadcasts on change.
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class POLYCHARACTERSRUNTIME_API UPolyElementalResistanceComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UPolyElementalResistanceComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category="Poly|Resistance")
	static UPolyElementalResistanceComponent* FindResistanceComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UPolyElementalResistanceComponent>() : nullptr); }

	UFUNCTION(BlueprintCallable, Category="Poly|Resistance")
	void InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category="Poly|Resistance")
	void UninitializeFromAbilitySystem();

	// Getters in normalized [0..1]
	UFUNCTION(BlueprintCallable, Category="Poly|Resistance")
	float GetResistRadiation() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Resistance")
	float GetResistIncendiary() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Resistance")
	float GetResistShock() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Resistance")
	float GetResistFreeze() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Resistance")
	float GetResistPoison() const;
	UFUNCTION(BlueprintCallable, Category="Poly|Resistance")
	float GetResistNanite() const;

public:
	UPROPERTY(BlueprintAssignable)
	FResistance_AttributeChanged OnResistanceChanged;

protected:
	virtual void OnUnregister() override;

	void HandleResistanceChanged(const FOnAttributeChangeData& ChangeData, const FGameplayAttribute& Attribute, const FGameplayTag& ResistanceTag);

	UPROPERTY()
	TObjectPtr<ULyraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const UPolyElementalResistanceSet> ResistanceSet;
};

