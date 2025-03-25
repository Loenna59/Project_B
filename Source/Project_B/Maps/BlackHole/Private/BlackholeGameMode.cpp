// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholeGameMode.h"

void ABlackholeGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto player = Cast<AActor>(NewPlayer->GetPawn());
	FVector newpos = player->GetActorLocation() + player->GetActorForwardVector()*300*playerIdx;
	player->SetActorLocation(newpos);
	playerIdx++;
}
