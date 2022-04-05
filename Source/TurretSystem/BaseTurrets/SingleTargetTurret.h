// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework\Actor.h"
#include "StatusSystem\BaseStatuses\IStatusBase.h"
#include "TurretSystem\BaseActors\BaseTurret.h"
#include "TurretSystem\BaseActors\BaseBullet.h"
#include "SingleTargetTurret.generated.h"

UCLASS()
class TURRETSYSTEM_API ASingleTargetTurret : public ABaseTurret
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASingleTargetTurret();

	UPROPERTY(BlueprintReadWrite)
	TArray<UBaseStatus*> Statuses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBulletStats BulletStats;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;




#pragma region ShootImplement
	
	UPROPERTY(EditInstanceOnly)
	float ShootDelay = 5;

	/** type of bullet */
	UPROPERTY(EditDefaultsOnly, Category=Damage)
	TSubclassOf<ABaseBullet> BulletType;

	float Delay = 0;

	virtual void Shoot(float DeltaTime) override;



#pragma endregion 
	
};
