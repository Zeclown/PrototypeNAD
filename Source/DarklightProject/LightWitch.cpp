// Fill out your copyright notice in the Description page of Project Settings.

#include "DarklightProject.h"
#include "LightWitch.h"

void ALightWitch::BeginPlay()
{
	Super::BeginPlay();
}

ALightWitch::ALightWitch()
{
	PrimaryActorTick.bCanEverTick = true;
}
void ALightWitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float Damage = (LIGHT_TRESHOLD/1000-LightExposition)*KillingRatio;
	if (Damage>0)
	{
		Health -= Damage;
		bIsInShadows = true;
	}
	else
	{
		Health = MaxHealth;
		bIsInShadows = false;
	}
}
