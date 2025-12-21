// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PolyAttributeSetLibrary.generated.h"
class ULyraAbilitySystemComponent;
class UPolyStaminaSet;
class UPolyElementalResistanceSet;
class UPolyCombatSet;
class UPolyStealthSet;
class UPolyHeatSet;
// Helper to ensure all Poly attribute sets are present on an ASC (call during pawn/feature init).
UCLASS()
class POLYCHARACTERSRUNTIME_API UPolyAttributeSetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Ensure all Poly attribute sets are present; returns true if ASC is valid. */
	UFUNCTION(BlueprintCallable, Category="Poly|AbilitySystem")
	static bool EnsurePolyAttributeSets(ULyraAbilitySystemComponent* LyraASC, UPolyStaminaSet*& OutStaminaSet, UPolyElementalResistanceSet*& OutResistanceSet, UPolyCombatSet*& OutCombatSet, UPolyStealthSet*& OutStealthSet, UPolyHeatSet*& OutHeatSet);
};

