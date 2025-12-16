// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

// Damage type tags (simple per-type identifiers)
POLYCHARACTERSRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Poly_Damage_Radiation);
POLYCHARACTERSRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Poly_Damage_Incendiary);
POLYCHARACTERSRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Poly_Damage_Shock);
POLYCHARACTERSRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Poly_Damage_Freeze);
POLYCHARACTERSRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Poly_Damage_Poison);
POLYCHARACTERSRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Poly_Damage_Nanite);

// SetByCaller damage magnitude tag used by damage effects
namespace PolyGameplayTags
{
	POLYCHARACTERSRUNTIME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
}

