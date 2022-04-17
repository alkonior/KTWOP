// Fill out your copyright notice in the Description page of Project Settings.


#include "IceDMG.h"

EStatusType UIceDMG::GetStatusType_Implementation()
{
	return IceDMG;
}

void UIceDMG::OnTick_Implementation(const TScriptInterface<IStatusOwner>& statusOwner, float dt)
{
	if (IsExploded)
	{
		if (AccumulatedDuration + dt >= StatusStats.Duration)
		{
			dt = StatusStats.Duration - AccumulatedDuration;
		}
		AccumulatedDuration += dt;
		if (AccumulatedDuration + FLT_EPSILON >= StatusStats.Duration)
		{
			statusOwner->Execute_RemoveStatus(statusOwner.GetObject(), IceDMG);
		}
		auto health = statusOwner->Execute_GetHealth(statusOwner.GetObject());
		auto maxHealth = statusOwner->Execute_GetMaxHealth(statusOwner.GetObject());

		if (maxHealth == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, dt, FColor::Red,
				FString::Printf(TEXT("MaxHealth is 0. WTF")
					));
			return;
		}
		auto percent = health/maxHealth;
		if (percent < IceStats.HealthDeathLimit)
		{
			statusOwner->Execute_ApplyDamage(statusOwner.GetObject(), health);
		}

		//TODO SlowDown

		
		ABaseEnemy* enemy = Cast<ABaseEnemy>(statusOwner.GetObject());
		if (IsValid(enemy))
		{
			GEngine->AddOnScreenDebugMessage(-1, dt, FColor::Red,
			    FString::Printf(TEXT("IceDMG  current percent: %f,  New health : %f"),
			    	 percent,  enemy->GetHealth_Implementation()));
			
			UE_LOG(LogTemp, Error, TEXT("IceDMG : %f,  New health : %f"),
					 percent,  enemy->GetHealth_Implementation());
		}
	}
	
}

void UIceDMG::OnDie_Implementation(const TScriptInterface<IStatusOwner>& statusOwner)
{}

void UIceDMG::Apply_Implementation(ABaseEnemy* enemy, FStatusModifier ExternalModifies)
{
	Super::Apply_Implementation(enemy, ExternalModifies);
}

void UIceDMG::AddToBullet_Implementation(AActor* enemy, FStatusModifier ExternalModifies)
{
	Super::AddToBullet_Implementation(enemy, ExternalModifies);
}

UBaseStatus* UIceDMG::MakeStatusCopy_Implementation(FStatusModifier ExternalModifies, UObject* outer)
{
	auto newStatus = NewObject<UIceDMG>(outer);
	newStatus->StatusStats = ApplyModifiersToStatusStats(StatusStats, ExternalModifies);
	newStatus->IceStats = IceStats;
	return newStatus;
}
