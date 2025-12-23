// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/PolyComboComponent.h"

#include "Engine/World.h"

UPolyComboComponent::UPolyComboComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	ComboTimeout = 2.0f;
	ComboCount = 0;
	LastHitTime = -1.0f;
	MaxComboRecorded = 0;
}

void UPolyComboComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPolyComboComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ComboCount > 0 && ComboTimeout > 0.0f)
	{
		const float Now = GetWorld()->GetTimeSeconds();
		if (Now - LastHitTime >= ComboTimeout)
		{
			ResetCombo();
		}
	}

	// Disable ticking when idle.
	if (ComboCount == 0)
	{
		SetComponentTickEnabled(false);
	}
}

void UPolyComboComponent::RegisterHit(float HitTimeSeconds, float MinDamageThreshold, float DamageAmount)
{
	// Optional filter: ignore trivial hits.
	if (MinDamageThreshold > 0.0f && DamageAmount < MinDamageThreshold)
	{
		return;
	}

	const float Now = (HitTimeSeconds >= 0.0f) ? HitTimeSeconds : GetWorld()->GetTimeSeconds();

	// If we timed out already, reset first.
	if (ComboCount > 0 && ComboTimeout > 0.0f && (Now - LastHitTime) >= ComboTimeout)
	{
		ResetCombo();
	}

	ComboCount += 1;
	LastHitTime = Now;
	MaxComboRecorded = FMath::Max(MaxComboRecorded, ComboCount);

	OnComboChanged.Broadcast(this, ComboCount);

	// Enable ticking while combo is active to watch for timeout.
	if (!IsComponentTickEnabled())
	{
		SetComponentTickEnabled(true);
	}
}

float UPolyComboComponent::GetTimeSinceLastHit() const
{
	if (LastHitTime < 0.0f)
	{
		return -1.0f;
	}
	return GetWorld() ? (GetWorld()->GetTimeSeconds() - LastHitTime) : -1.0f;
}

void UPolyComboComponent::ResetCombo()
{
	if (ComboCount != 0)
	{
		ComboCount = 0;
		OnComboReset.Broadcast(this);
	}
	LastHitTime = -1.0f;
}




