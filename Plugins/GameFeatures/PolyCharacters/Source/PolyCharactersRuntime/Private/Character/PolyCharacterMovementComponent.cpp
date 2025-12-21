// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/PolyCharacterMovementComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/Attributes/PolyStaminaSet.h"
#include "GameplayTagContainer.h"
#include "LyraGameplayTags.h"

float UPolyCharacterMovementComponent::GetMaxSpeed() const
{
	// Preserve Lyra's movement-stopped tag handling.
	if (const UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if (ASC->HasMatchingGameplayTag(LyraGameplayTags::Status_MovementStopped))
		{
			return 0.0f;
		}

		if (const UPolyStaminaSet* StaminaSet = ASC->GetSet<UPolyStaminaSet>())
		{
			const float Speed = StaminaSet->GetMoveSpeed();
			if (Speed > 0.0f)
			{
				return Speed;
			}
		}
	}

	return Super::GetMaxSpeed();
}

