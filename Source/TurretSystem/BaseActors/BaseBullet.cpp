// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBullet.h"

#include "Components/SphereComponent.h"
#include "EnemySystem/Enemy/BaseEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ABaseBullet::ABaseBullet()
{
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
	
	PrimaryActorTick.bCanEverTick = true;
	
	//AbilitySystemComponent = CreateDefaultSubobject<UBulletAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//AbilitySystemComponent->SetIsReplicated(true);


	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetHiddenInGame(false);
	CollisionComponent->SetCollisionProfileName(TEXT("EnemyDestroyer"));

	
}

void ABaseBullet::Init(const TArray<UBaseStatus*>& statuses, AActor* targetEnemy, FBaseBulletStats baseStats, FBulletStats bulletStats) {

	this->Statuses = statuses;
	this->TargetEnemy = Cast<ABaseEnemy>(targetEnemy);
	this->BaseStats = baseStats;
	this->BulletStats = bulletStats;
	CollisionComponent->SetSphereRadius(this->BulletStats.BulletRadius);
	
}
// Called when the game starts or when spawned
void ABaseBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseBullet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseBullet::OnBeginOverlap);
}

void ABaseBullet::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{



	ABaseEnemy* enemy = Cast<ABaseEnemy>(OtherActor);
	if (IsValid(enemy) && (!VisitedEnemies.Contains(enemy)))
	{
		VisitedEnemies.Add(enemy);
		
		for (auto& status :Statuses)
		{
			status->Apply(enemy, BulletStats.StatusModifies);
		}
		if (BaseStats.BounceCount > 0)
		{
			TArray<FOverlapResult> EnemyOverlaps;
			FCollisionQueryParams QueryParams("", false);
			
			FCollisionResponseParams ResponseParams;
			ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
			ResponseParams.CollisionResponse.SetResponse(EnemyDestroyerCollisionChannel, ECR_Overlap);
			ResponseParams.CollisionResponse.SetResponse(TurretEnemyCollisionChannel, ECR_Overlap);

			GetWorld()->OverlapMultiByChannel(
				EnemyOverlaps,
				RootComponent->GetComponentLocation(),
				FQuat::Identity,
				EnemyDestroyerCollisionChannel,
				FCollisionShape::MakeSphere(FMath::Max(BaseStats.BounceRadius, BaseStats.MaxFlyDistance - SpentFlyDistance)),
				QueryParams,
				ResponseParams
				);

			TArray<ABaseEnemy*> GoodEnemies;
			for (auto& collision : EnemyOverlaps)
			{
				ABaseEnemy* enemyActor = Cast<ABaseEnemy>(collision.GetActor());
				if (IsValid(enemyActor) && (!VisitedEnemies.Contains(enemyActor)))
				{
					GoodEnemies.Add(enemyActor);
				}
			}
			
			bool foundTarget = false;
			if (GoodEnemies.Num() > 0)
			{
				FVector position = RootComponent->GetComponentLocation();
				
				float distance = BaseStats.MaxFlyDistance - SpentFlyDistance;
				for (auto&  enemyActor : GoodEnemies)
				{
					if ((position-enemyActor->GetPosition()).Size() < distance)
					{
						TargetEnemy = enemyActor;
						foundTarget = true;
						distance = (position-enemyActor->GetPosition()).Size();
					}
				}
			}
			
			if (!foundTarget)
			{
				BaseStats.BounceCount = 0;
			} else
			{
				BulletStats.StatusModifies = BulletStats.StatusModifies+BaseStats.BounceModifier;
				BaseStats.BounceCount--;
			}
		}
		OnTargetHit(OverlappedComponent, OtherActor, OtherComponent,  OtherBodyIndex, bFromSweep, Hit);
	}
}

void ABaseBullet::StartFly()
{
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(
		TargetEnemy->GetPosition(),
		GetActorLocation() + FVector(0, 0, 0));
	SetActorRotation(rotation);
	LastDirection = rotation.Vector();
	IsWorking = true;
}

void ABaseBullet::StopFly()
{
	IsWorking = false;
}

// Called every frame
void ABaseBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsWorking)
		return;

	
	FVector bulletLocation =  RootComponent->GetComponentLocation();
	auto flyDistance = BulletStats.BulletSpeed*DeltaTime*100;
	
	if (IsValid(TargetEnemy) && !TargetEnemy->IsDead_Implementation())
	{
		
		FVector targetLocation =  TargetEnemy->GetPosition();
		auto maxDistance = (targetLocation-bulletLocation).Size();

		LastDirection = (targetLocation-bulletLocation).GetSafeNormal();
		
		flyDistance = FMath::Clamp<float>(BulletStats.BulletSpeed*DeltaTime*100, 0, maxDistance);
		
	}
	
	FVector newLocation = bulletLocation + LastDirection*flyDistance;
	
	SpentFlyDistance += (bulletLocation - newLocation).Size();
	
	SetActorRotation(LastDirection.Rotation());

	if (SpentFlyDistance > BaseStats.MaxFlyDistance)
	{
		OnDistanceDeplete();
		return;
	}
	
	RootComponent->SetWorldLocation(newLocation);
	
}

