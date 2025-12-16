// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Executions/PolyDamageExecution.h"

#include "AbilitySystem/Attributes/PolyElementalResistanceSet.h"
#include "AbilitySystem/Attributes/PolyCombatSet.h"
#include "AbilitySystem/Attributes/LyraHealthSet.h"
#include "AbilitySystemComponent.h"
#include "PolyGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PolyDamageExecution)

namespace PolyDamageExec
{
	// Captures for per-type resistances on the target.
	static FGameplayEffectAttributeCaptureDefinition ResistRadiationDef;
	static FGameplayEffectAttributeCaptureDefinition ResistIncendiaryDef;
	static FGameplayEffectAttributeCaptureDefinition ResistShockDef;
	static FGameplayEffectAttributeCaptureDefinition ResistFreezeDef;
	static FGameplayEffectAttributeCaptureDefinition ResistPoisonDef;
	static FGameplayEffectAttributeCaptureDefinition ResistNaniteDef;

	// Combat attributes
	static FGameplayEffectAttributeCaptureDefinition DamagePowerDef;
	static FGameplayEffectAttributeCaptureDefinition CritChanceDef;
	static FGameplayEffectAttributeCaptureDefinition CritMultiplierDef;
	static FGameplayEffectAttributeCaptureDefinition FlatMitigationDef;
	static FGameplayEffectAttributeCaptureDefinition BlockChanceDef;
	static FGameplayEffectAttributeCaptureDefinition BlockAmountDef;
	static FGameplayEffectAttributeCaptureDefinition ThornsDef;
	static FGameplayEffectAttributeCaptureDefinition LifeStealDef;
	static FGameplayEffectAttributeCaptureDefinition ShieldGainOnHitDef;

	static void InitCaptureDefs()
	{
		if (ResistRadiationDef.AttributeToCapture.IsValid())
		{
			return;
		}

		ResistRadiationDef = FGameplayEffectAttributeCaptureDefinition(UPolyElementalResistanceSet::GetResistRadiationAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		ResistIncendiaryDef = FGameplayEffectAttributeCaptureDefinition(UPolyElementalResistanceSet::GetResistIncendiaryAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		ResistShockDef = FGameplayEffectAttributeCaptureDefinition(UPolyElementalResistanceSet::GetResistShockAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		ResistFreezeDef = FGameplayEffectAttributeCaptureDefinition(UPolyElementalResistanceSet::GetResistFreezeAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		ResistPoisonDef = FGameplayEffectAttributeCaptureDefinition(UPolyElementalResistanceSet::GetResistPoisonAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		ResistNaniteDef = FGameplayEffectAttributeCaptureDefinition(UPolyElementalResistanceSet::GetResistNaniteAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);

		// Source offense/sustain
		DamagePowerDef    = FGameplayEffectAttributeCaptureDefinition(UPolyCombatSet::GetDamagePowerAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
		CritChanceDef     = FGameplayEffectAttributeCaptureDefinition(UPolyCombatSet::GetCritChanceAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
		CritMultiplierDef = FGameplayEffectAttributeCaptureDefinition(UPolyCombatSet::GetCritMultiplierAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
		LifeStealDef      = FGameplayEffectAttributeCaptureDefinition(UPolyCombatSet::GetLifeStealAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
		ShieldGainOnHitDef= FGameplayEffectAttributeCaptureDefinition(UPolyCombatSet::GetShieldGainOnHitAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);

		// Target mitigation/reactive
		FlatMitigationDef = FGameplayEffectAttributeCaptureDefinition(UPolyCombatSet::GetFlatMitigationAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		BlockChanceDef    = FGameplayEffectAttributeCaptureDefinition(UPolyCombatSet::GetBlockChanceAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		BlockAmountDef    = FGameplayEffectAttributeCaptureDefinition(UPolyCombatSet::GetBlockAmountAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
		ThornsDef         = FGameplayEffectAttributeCaptureDefinition(UPolyCombatSet::GetThornsAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	}

	static FGameplayTag FindDamageType(const FGameplayEffectSpec& Spec)
	{
		const FGameplayTagContainer& SourceTags = *Spec.CapturedSourceTags.GetAggregatedTags();
		const FGameplayTagContainer& TargetTags = *Spec.CapturedTargetTags.GetAggregatedTags();

		// Check both source and target tags for explicit damage type markers.
		const FGameplayTag KnownTypes[] =
		{
			TAG_Poly_Damage_Radiation,
			TAG_Poly_Damage_Incendiary,
			TAG_Poly_Damage_Shock,
			TAG_Poly_Damage_Freeze,
			TAG_Poly_Damage_Poison,
			TAG_Poly_Damage_Nanite
		};

		for (const FGameplayTag& DamageTypeTag : KnownTypes)
		{
			if (SourceTags.HasTag(DamageTypeTag) || TargetTags.HasTag(DamageTypeTag))
			{
				return DamageTypeTag;
			}
		}

		return FGameplayTag();
	}

	static float GetResistanceForType(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& CaptureDef)
	{
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, FAggregatorEvaluateParameters(), Resistance);
		return FMath::Clamp(Resistance, 0.f, 1.f);
	}

	static float GetClamped0To1(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& CaptureDef)
	{
		float Value = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, FAggregatorEvaluateParameters(), Value);
		return FMath::Clamp(Value, 0.f, 1.f);
	}

	static float GetClampedMin(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& CaptureDef, float MinValue)
	{
		float Value = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, FAggregatorEvaluateParameters(), Value);
		return FMath::Max(Value, MinValue);
	}
}

UPolyDamageExecution::UPolyDamageExecution()
{
	PolyDamageExec::InitCaptureDefs();

	RelevantAttributesToCapture.Add(PolyDamageExec::ResistRadiationDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::ResistIncendiaryDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::ResistShockDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::ResistFreezeDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::ResistPoisonDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::ResistNaniteDef);

	RelevantAttributesToCapture.Add(PolyDamageExec::DamagePowerDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::CritChanceDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::CritMultiplierDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::FlatMitigationDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::BlockChanceDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::BlockAmountDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::ThornsDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::LifeStealDef);
	RelevantAttributesToCapture.Add(PolyDamageExec::ShieldGainOnHitDef);
}

void UPolyDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Expect callers to set damage via SetByCaller.Poly.Damage.
	const float RawDamage = Spec.GetSetByCallerMagnitude(PolyGameplayTags::SetByCaller_Damage, false, 0.0f);
	if (RawDamage <= 0.0f)
	{
		return;
	}

	const FGameplayTag DamageType = PolyDamageExec::FindDamageType(Spec);

	float Resistance = 0.0f;
	if (DamageType.IsValid())
	{
		if (DamageType == TAG_Poly_Damage_Radiation)
		{
			Resistance = PolyDamageExec::GetResistanceForType(ExecutionParams, PolyDamageExec::ResistRadiationDef);
		}
		else if (DamageType == TAG_Poly_Damage_Incendiary)
		{
			Resistance = PolyDamageExec::GetResistanceForType(ExecutionParams, PolyDamageExec::ResistIncendiaryDef);
		}
		else if (DamageType == TAG_Poly_Damage_Shock)
		{
			Resistance = PolyDamageExec::GetResistanceForType(ExecutionParams, PolyDamageExec::ResistShockDef);
		}
		else if (DamageType == TAG_Poly_Damage_Freeze)
		{
			Resistance = PolyDamageExec::GetResistanceForType(ExecutionParams, PolyDamageExec::ResistFreezeDef);
		}
		else if (DamageType == TAG_Poly_Damage_Poison)
		{
			Resistance = PolyDamageExec::GetResistanceForType(ExecutionParams, PolyDamageExec::ResistPoisonDef);
		}
		else if (DamageType == TAG_Poly_Damage_Nanite)
		{
			Resistance = PolyDamageExec::GetResistanceForType(ExecutionParams, PolyDamageExec::ResistNaniteDef);
		}
	}

	// Source multipliers
	const float DamagePower  = PolyDamageExec::GetClampedMin(ExecutionParams, PolyDamageExec::DamagePowerDef, 0.f);
	const float CritChance   = PolyDamageExec::GetClamped0To1(ExecutionParams, PolyDamageExec::CritChanceDef);
	const float CritMult     = PolyDamageExec::GetClampedMin(ExecutionParams, PolyDamageExec::CritMultiplierDef, 1.f);

	// Target mitigation/reactive
	const float FlatMit      = PolyDamageExec::GetClampedMin(ExecutionParams, PolyDamageExec::FlatMitigationDef, 0.f);
	const float BlockChance  = PolyDamageExec::GetClamped0To1(ExecutionParams, PolyDamageExec::BlockChanceDef);
	const float BlockAmount  = PolyDamageExec::GetClamped0To1(ExecutionParams, PolyDamageExec::BlockAmountDef);
	const float Thorns       = PolyDamageExec::GetClamped0To1(ExecutionParams, PolyDamageExec::ThornsDef);

	// Source sustain
	const float LifeSteal    = PolyDamageExec::GetClamped0To1(ExecutionParams, PolyDamageExec::LifeStealDef);
	const float ShieldGain   = PolyDamageExec::GetClamped0To1(ExecutionParams, PolyDamageExec::ShieldGainOnHitDef);

	// Power scale
	float WorkingDamage = RawDamage * (DamagePower <= 0.f ? 0.f : DamagePower);

	// Crit roll
	if (CritChance > 0.f)
	{
		const float Roll = FMath::FRand();
		if (Roll <= CritChance)
		{
			WorkingDamage *= CritMult;
		}
	}

	// Flat mitigation before resistances
	WorkingDamage = FMath::Max(0.f, WorkingDamage - FlatMit);

	// Block check
	if (BlockChance > 0.f)
	{
		const float Roll = FMath::FRand();
		if (Roll <= BlockChance)
		{
			WorkingDamage *= (1.0f - BlockAmount);
		}
	}

	// Apply elemental resistance
	const float FinalDamage = WorkingDamage * (1.0f - Resistance);
	if (FinalDamage <= 0.0f)
	{
		return;
	}

	// Route through Lyra's damage meta so shield/health logic applies.
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(ULyraHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, FinalDamage));

	// Note: Execution can only modify the target. Apply thorns/lifesteal/shield gain via follow-up effects or cues if needed.
}

