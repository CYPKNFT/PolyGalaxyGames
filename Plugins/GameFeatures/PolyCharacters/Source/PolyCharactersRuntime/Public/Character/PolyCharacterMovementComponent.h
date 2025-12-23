// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Character/LyraCharacterMovementComponent.h"

#include "PolyCharacterMovementComponent.generated.h"

class UPolyStaminaSet;

/**
 * Movement component that sources max speed from PolyStaminaSet::MoveSpeed.
 */
UCLASS()
class POLYCHARACTERSRUNTIME_API UPolyCharacterMovementComponent : public ULyraCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual float GetMaxSpeed() const override;
};

