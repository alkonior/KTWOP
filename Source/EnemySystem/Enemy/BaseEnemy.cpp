﻿#include "BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetHiddenInGame(false);
	CollisionComponent->SetCollisionProfileName(TEXT("TurretEnemy"));
	CollisionComponent->SetSphereRadius(EnemyStats.CollisionRadius);
}

void ABaseEnemy::AddStatus(TScriptInterface<IStatusBase> status)
{
	// TODO: check, if this status already exist and so on...
	EStatusType type = IStatusBase::Execute_GetStatusType(status.GetObject());
	StatusesMap.Add(type, status);
}

void ABaseEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	HashedSpeedModifier = EnemyStats.SpeedModifier;
	HashedDmgModifier = EnemyStats.DmgModifier;
}

void ABaseEnemy::Init(const FEnemyStats& Stats)
{
	EnemyStats = Stats;
	EnemyStats.Health = EnemyStats.MaxHealth;
	HashedSpeedModifier = EnemyStats.SpeedModifier;
	HashedDmgModifier = EnemyStats.DmgModifier;
}

void ABaseEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	TScriptInterface<IStatusOwner> statusOwner;
	statusOwner.SetInterface(this);
	statusOwner.SetObject(this);
	
	for (const auto& pair : StatusesMap)
	{
		IStatusBase::Execute_OnTick(pair.Value.GetObject(), statusOwner, DeltaSeconds);
	}
	for (auto& removedStatus : RemovedStatuses)
	{
		StatusesMap.Remove(removedStatus);
	}
	RemovedStatuses.Empty();
}

FVector ABaseEnemy::GetPosition()
{
	return CollisionComponent->GetComponentLocation();
}

bool ABaseEnemy::HasStatus_Implementation(EStatusType statusType)
{
	return StatusesMap.Contains(statusType);
}

void ABaseEnemy::RemoveStatus_Implementation(EStatusType statusType)
{
	RemovedStatuses.Add(statusType);
}

TScriptInterface<IStatusData> ABaseEnemy::GetStatus_Implementation(EStatusType statusType)
{
	if (StatusesMap.Contains(TEnumAsByte<EStatusType>(statusType)))
	{
		TScriptInterface<IStatusBase>& statusBase = StatusesMap[TEnumAsByte<EStatusType>(statusType)];
		TScriptInterface<IStatusData> statusData;
		statusData.SetInterface(Cast<IStatusData>(statusBase.GetObject()));
		statusData.SetObject(statusBase.GetObject());
		return statusData;
	}
	return nullptr;
}

float ABaseEnemy::GetHealth_Implementation()
{
	return EnemyStats.Health;
}

float ABaseEnemy::GetMaxHealth_Implementation()
{
	return EnemyStats.MaxHealth;
}

void ABaseEnemy::ApplyDamage_Implementation(float damage)
{
	damage = EnemyStats.Health - FMath::Clamp<float>(EnemyStats.Health - damage * GetDmgModifier_Implementation(), 0.0f, EnemyStats.MaxHealth );
	ShowApplyedDamage(damage);
	
	for (const TTuple<TEnumAsByte<EStatusType>, TScriptInterface<IStatusBase>>& pair: StatusesMap)
	{
		IStatusBase::Execute_ApplyDmg(pair.Value.GetObject(), damage);
	}
	EnemyStats.Health -= damage;
	BP_DamageApplied(damage, EnemyStats.Health / EnemyStats.MaxHealth);
	
	TScriptInterface<IStatusOwner> statusOwner;
	statusOwner.SetInterface(this);
	statusOwner.SetObject(this);
	
	if (EnemyStats.Health <= 1)
	{
		for (const TTuple<TEnumAsByte<EStatusType>, TScriptInterface<IStatusBase>>& pair: StatusesMap)
		{
			IStatusBase::Execute_OnDie(pair.Value.GetObject(), statusOwner);
		}
		
		this->Execute_OnDeath(this);
	}
}

bool ABaseEnemy::IsArmored_Implementation()
{
	return EnemyStats.IsArmored;
}

void ABaseEnemy::SetHealth_Implementation(float newHealth)
{
	EnemyStats.Health = newHealth;
}

void ABaseEnemy::ApplySpeedModifier_Implementation(float modifier, EStatusType status)
{
	SpeedModifierMap.Add(status, modifier);
	float speedM = EnemyStats.SpeedModifier;
	for (auto sm : SpeedModifierMap)
	{
		speedM *= sm.Value;
	}
	HashedSpeedModifier = speedM;
}

void ABaseEnemy::ApplyDmgModifier_Implementation(float modifier, EStatusType status)
{
	DmgModifierMap.Add(status, modifier);
	float damageM = EnemyStats.DmgModifier;
	for (auto dm : DmgModifierMap)
	{
		damageM *= dm.Value;
	}
	HashedDmgModifier = damageM;
}

FVector ABaseEnemy::GetLocation_Implementation()
{
	return GetPosition();
}

float ABaseEnemy::GetSpeedModifier_Implementation()
{
	return HashedSpeedModifier;
}

float ABaseEnemy::GetDmgModifier_Implementation()
{
	return HashedDmgModifier;
}

bool ABaseEnemy::IsDead_Implementation()
{
	return EnemyStats.Health < FLT_EPSILON;
}
