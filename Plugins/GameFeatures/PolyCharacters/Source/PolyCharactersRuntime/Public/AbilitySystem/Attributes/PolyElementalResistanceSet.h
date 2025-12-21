// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attributes/LyraAttributeSet.h"
#include "AbilitySystemComponent.h"

#include "PolyElementalResistanceSet.generated.h"

// Elemental resistances (Radiation/Incendiary/Shock/Freeze/Poison/Nanite), all clamped 0..1. Replicated for clients; read by PolyDamageExecution to scale incoming elemental damage.
UCLASS(BlueprintType)
class POLYCHARACTERSRUNTIME_API UPolyElementalResistanceSet : public ULyraAttributeSet
{
	GENERATED_BODY()

public:
	UPolyElementalResistanceSet();

	// Scalar resistances per damage type, expected 0..1
	ATTRIBUTE_ACCESSORS(UPolyElementalResistanceSet, ResistRadiation);
	ATTRIBUTE_ACCESSORS(UPolyElementalResistanceSet, ResistIncendiary);
	ATTRIBUTE_ACCESSORS(UPolyElementalResistanceSet, ResistShock);
	ATTRIBUTE_ACCESSORS(UPolyElementalResistanceSet, ResistFreeze);
	ATTRIBUTE_ACCESSORS(UPolyElementalResistanceSet, ResistPoison);
	ATTRIBUTE_ACCESSORS(UPolyElementalResistanceSet, ResistNanite);

protected:
	UFUNCTION()
	void OnRep_ResistRadiation(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_ResistIncendiary(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_ResistShock(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_ResistFreeze(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_ResistPoison(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_ResistNanite(const FGameplayAttributeData& OldValue);

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ResistRadiation, Category="Poly|Resistance", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData ResistRadiation;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ResistIncendiary, Category="Poly|Resistance", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData ResistIncendiary;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ResistShock, Category="Poly|Resistance", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData ResistShock;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ResistFreeze, Category="Poly|Resistance", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData ResistFreeze;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ResistPoison, Category="Poly|Resistance", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData ResistPoison;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ResistNanite, Category="Poly|Resistance", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData ResistNanite;
};

