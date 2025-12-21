// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"

#include "PolyStaminaComponent.generated.h"

struct FOnAttributeChangeData;
class ULyraAbilitySystemComponent;
class UPolyStaminaSet;

// Fired when stamina/max stamina changes.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStamina_AttributeChanged, UPolyStaminaComponent*, StaminaComponent, float, OldValue, float, NewValue);

// Component exposing stamina attributes and change delegates (init with ASC). Listens to PolyStaminaSet and provides getters/normalized values.
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class POLYCHARACTERSRUNTIME_API UPolyStaminaComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	UPolyStaminaComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category="Poly|Stamina")
	static UPolyStaminaComponent* FindStaminaComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UPolyStaminaComponent>() : nullptr); }

	UFUNCTION(BlueprintCallable, Category="Poly|Stamina")
	void InitializeWithAbilitySystem(ULyraAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable, Category="Poly|Stamina")
	void UninitializeFromAbilitySystem();

	UFUNCTION(BlueprintCallable, Category="Poly|Stamina")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category="Poly|Stamina")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category="Poly|Stamina")
	float GetStaminaNormalized() const;

public:
	UPROPERTY(BlueprintAssignable)
	FStamina_AttributeChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FStamina_AttributeChanged OnMaxStaminaChanged;

protected:
	virtual void OnUnregister() override;

	void HandleStaminaChanged(const FOnAttributeChangeData& ChangeData);
	void HandleMaxStaminaChanged(const FOnAttributeChangeData& ChangeData);

	UPROPERTY()
	TObjectPtr<ULyraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const UPolyStaminaSet> StaminaSet;
};

