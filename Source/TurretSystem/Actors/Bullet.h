// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework\Actor.h"
#include "TurretSystem\BulletAbilitySystemComponent.h"
#include "Bullet.generated.h"

UCLASS()
class TURRETSYSTEM_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

	UPROPERTY(BlueprintReadOnly)
	UBulletAbilitySystemComponent* AbilitySystemComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
