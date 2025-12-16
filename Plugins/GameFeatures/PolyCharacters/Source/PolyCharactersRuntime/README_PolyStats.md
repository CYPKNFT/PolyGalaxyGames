# PolyCharacters Runtime – Stats & Damage (C++)

Overview

- Uses Lyra’s `LyraHealthSet` for health/shield and routes damage through its Damage meta attribute.
- Adds Poly attribute sets:
  - `PolyStaminaSet`: Stamina/MaxStamina/Recuperating with clamps, replication, delegates.
    - Sprint: MoveSpeed (absolute sprint speed), SprintStaminaCostPerSecond (>=0), SprintStaminaMinToStart (>=0).
  - `PolyElementalResistanceSet`: Per-type resistances (Radiation/Incendiary/Shock/Freeze/Poison/Nanite) clamped 0..1.
  - `PolyCombatSet`: Offense/mitigation/utility/regens (DamagePower, CritChance, CritMultiplier, FlatMitigation, BlockChance, BlockAmount, Thorns, LifeSteal, ShieldGainOnHit, Tenacity, MoveSpeedScalar, Health/Stamina/Shield regen).
  - `PolyStealthSet`: StealthFactor, DetectionResistance, InvisibilityStrength, RevealResistance, NoiseScalar, LightVisibilityScalar.
  - `PolyHeatSet`: WantedLevel (0..7), HeatDecayRate, CrimeNotoriety, BribeCostScalar.
- Damage execution: `PolyDamageExecution` consumes `SetByCaller.Poly.Damage` + one `Poly.Damage.*` tag, applies power/crit/flatMit/block/resistance, then thorns + lifesteal/shield gain; outputs to `LyraHealthSet::Damage`.

Initialization

- Call `UPolyAttributeSetLibrary::EnsurePolyAttributeSets(ASC, OutStamina, OutResist, OutCombat)` to attach the sets to a `ULyraAbilitySystemComponent`.
- Components:
  - `PolyStaminaComponent` exposes stamina values and delegates.
  - `PolyElementalResistanceComponent` exposes resistances and delegates.
  - `PolyCombatComponent` exposes combat attributes and delegates.

Gameplay Effects (author in data)

- Instant GE with execution `PolyDamageExecution`.
- Set `SetByCaller.Poly.Damage` magnitude.
- Add exactly one damage type tag: `Poly.Damage.Radiation|Incendiary|Shock|Freeze|Poison|Nanite`.

- Attributes (key clamps)

  - Resistances, chances, fractions: 0..1 (CritChance, BlockChance, BlockAmount, Thorns, LifeSteal, ShieldGainOnHit, Tenacity, elemental resistances).
  - CritMultiplier ≥ 1.
  - MoveSpeedScalar > 0.
  - Power/FlatMit/Regen ≥ 0.
  - Stealth: StealthFactor ≥0 (capped ~2), DetectionResistance/InvisibilityStrength/RevealResistance/NoiseScalar/LightVisibilityScalar clamped 0..1.
  - Heat: WantedLevel 0..7; HeatDecayRate/CrimeNotoriety/BribeCostScalar ≥0.

Flow

1) Power scale → Crit → FlatMit → Block → Elemental resistance → Final damage to Lyra damage meta.
2) Thorns applies to source based on final damage.
3) LifeSteal and ShieldGainOnHit apply to source based on final damage.
