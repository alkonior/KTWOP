﻿#include "IStatusBase.h"

void IStatusBase::Apply_Implementation(TScriptInterface<IStatusOwner>& statusOwner)
{
	UE_LOG(LogTemp, Error, TEXT("\"Apply\" method isn't implemented."));
}

void IStatusBase::OnTick_Implementation(TScriptInterface<IStatusOwner>& statusOwner)
{
	UE_LOG(LogTemp, Error, TEXT("\"OnTick\" method isn't implemented."));
}

void IStatusBase::OnDie_Implementation(TScriptInterface<IStatusOwner>& statusOwner)
{
	UE_LOG(LogTemp, Error, TEXT("\"OnDie\" method isn't implemented."));
}


TScriptInterface<IStatusBase> IStatusBase::MakeStatusCopy_Implementation(float ExternalModifier)
{
	UE_LOG(LogTemp, Error, TEXT("\"MakeStatusCopy\" method isn't implemented."));
	return nullptr;
}