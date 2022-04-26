// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IShootable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UShootable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURRETSYSTEM_API IShootable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void Shoot(float DeltaTime);

	
};
