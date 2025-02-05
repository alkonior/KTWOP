﻿#pragma once

#include "CoreMinimal.h"
#include "StatusType.h"
#include "UObject/Interface.h"
#include "IStatusData.generated.h"

UINTERFACE()
class STATUSSYSTEM_API UStatusData : public UInterface
{
	GENERATED_BODY()	
};

class STATUSSYSTEM_API IStatusData
{
	GENERATED_BODY()
	
public:
#pragma region Modifier
	
	// Turret modifier.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetModifier();
	UFUNCTION()
	virtual float GetModifier_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetModifier(float modifier);
	UFUNCTION()
	virtual void SetModifier_Implementation(float modifier);

#pragma endregion 

#pragma region Power
	
	// Power of status.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetPower();
	UFUNCTION()
	virtual float GetPower_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPower(float power);
	UFUNCTION()
	virtual void SetPower_Implementation(float power);

#pragma endregion

#pragma region Duration

	// Duration of status.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetDuration();
	UFUNCTION()
	virtual float GetDuration_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDuration(float duration);
	UFUNCTION()
	virtual void SetDuration_Implementation(float duration);

#pragma endregion
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EStatusType GetStatusType();
	
	UFUNCTION()
	virtual EStatusType GetStatusType_Implementation();
};



UINTERFACE(BlueprintType)
class STATUSSYSTEM_API UPositionedActor : public UInterface
{
	GENERATED_BODY()	
};


class STATUSSYSTEM_API IPositionedActor
{
	GENERATED_BODY()
	
public:

	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetLocation();
	
	UFUNCTION()
	virtual FVector GetLocation_Implementation();

};