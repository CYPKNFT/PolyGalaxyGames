// Copyright Epic Games, Inc. All Rights Reserved.
#include "AbilitySystem/PolyAttributeSetLibrary.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/PolyStaminaSet.h"
#include "AbilitySystem/Attributes/PolyElementalResistanceSet.h"
#include "AbilitySystem/Attributes/PolyCombatSet.h"
#include "AbilitySystem/Attributes/PolyStealthSet.h"
#include "AbilitySystem/Attributes/PolyHeatSet.h"

bool UPolyAttributeSetLibrary::EnsurePolyAttributeSets(ULyraAbilitySystemComponent* LyraASC, UPolyStaminaSet*& OutStaminaSet, UPolyElementalResistanceSet*& OutResistanceSet, UPolyCombatSet*& OutCombatSet, UPolyStealthSet*& OutStealthSet, UPolyHeatSet*& OutHeatSet)
{
	OutStaminaSet = nullptr;
	OutResistanceSet = nullptr;
	OutCombatSet = nullptr;
	OutStealthSet = nullptr;
	OutHeatSet = nullptr;

	if (!LyraASC)
	{
		return false;
	}

	// Try existing sets first.
	OutStaminaSet = const_cast<UPolyStaminaSet*>(LyraASC->GetSet<UPolyStaminaSet>());
	OutResistanceSet = const_cast<UPolyElementalResistanceSet*>(LyraASC->GetSet<UPolyElementalResistanceSet>());
	OutCombatSet = const_cast<UPolyCombatSet*>(LyraASC->GetSet<UPolyCombatSet>());
	OutStealthSet = const_cast<UPolyStealthSet*>(LyraASC->GetSet<UPolyStealthSet>());
	OutHeatSet = const_cast<UPolyHeatSet*>(LyraASC->GetSet<UPolyHeatSet>());

	// Create and add if missing.
	if (!OutStaminaSet)
	{
		OutStaminaSet = NewObject<UPolyStaminaSet>(LyraASC);
		LyraASC->AddAttributeSetSubobject(OutStaminaSet);
	}

	if (!OutResistanceSet)
	{
		OutResistanceSet = NewObject<UPolyElementalResistanceSet>(LyraASC);
		LyraASC->AddAttributeSetSubobject(OutResistanceSet);
	}

	if (!OutCombatSet)
	{
		OutCombatSet = NewObject<UPolyCombatSet>(LyraASC);
		LyraASC->AddAttributeSetSubobject(OutCombatSet);
	}

	if (!OutStealthSet)
	{
		OutStealthSet = NewObject<UPolyStealthSet>(LyraASC);
		LyraASC->AddAttributeSetSubobject(OutStealthSet);
	}

	if (!OutHeatSet)
	{
		OutHeatSet = NewObject<UPolyHeatSet>(LyraASC);
		LyraASC->AddAttributeSetSubobject(OutHeatSet);
	}

	return true;
}

