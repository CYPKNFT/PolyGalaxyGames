// Copyright Epic Games, Inc. All Rights Reserved.
+
#pragma once
+
#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
+
#include "PolyComboComponent.generated.h"
+
/**
 * Tracks hit combos over time: increments on RegisterHit, resets after ComboTimeout with no hits.
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class POLYCHARACTERSRUNTIME_API UPolyComboComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()
+
public:
	UPolyComboComponent(const FObjectInitializer& ObjectInitializer);
+
	// Register a hit to advance the combo. Call from your damage/hit logic (server-side).
	UFUNCTION(BlueprintCallable, Category="Poly|Combo")
	void RegisterHit(float HitTimeSeconds = -1.0f, float MinDamageThreshold = 0.0f, float DamageAmount = 0.0f);
+
	UFUNCTION(BlueprintPure, Category="Poly|Combo")
	int32 GetComboCount() const { return ComboCount; }
+
	UFUNCTION(BlueprintPure, Category="Poly|Combo")
	float GetTimeSinceLastHit() const;
+
	// Seconds of inactivity before combo resets.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Poly|Combo")
	float ComboTimeout;
+
	// Delegate fired when combo count changes.
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComboChanged, UPolyComboComponent*, ComboComp, int32, NewComboCount);
	UPROPERTY(BlueprintAssignable, Category="Poly|Combo")
	FOnComboChanged OnComboChanged;
+
	// Delegate fired when combo resets (timeout or explicit reset).
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboReset, UPolyComboComponent*, ComboComp);
	UPROPERTY(BlueprintAssignable, Category="Poly|Combo")
	FOnComboReset OnComboReset;
+
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
+
	// Reset combo to zero and notify.
	void ResetCombo();H
+
	UPROPERTY(VisibleInstanceOnly, Category="Poly|Combo")
	int32 ComboCount;
+
	UPROPERTY(VisibleInstanceOnly, Category="Poly|Combo")
	float LastHitTime;
+
	// Track the highest combo seen (for UI/telemetry if desired).
	UPROPERTY(VisibleInstanceOnly, Category="Poly|Combo")
	int32 MaxComboRecorded;
};

