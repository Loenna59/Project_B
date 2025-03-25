// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_B/Maps/BlackHole/Public/BlackholePlayerState.h"

#include "Net/UnrealNetwork.h"

void ABlackholePlayerState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlackholePlayerState, TeamID);
}

void ABlackholePlayerState::SetTeam(int32 NewTeamID)
{
	// 서버에서만 실행
	if (HasAuthority())
	{
		TeamID = NewTeamID;
		// 클라이언트에게 팀 정보 동기화!
		OnRep_TeamID(); 
	}
}

void ABlackholePlayerState::OnRep_TeamID()
{
	// UI 업데이트 또는 팀 색상 변경을 여기서 수행하기를.... 지피티가 추천함
	UE_LOG(LogTemp, Log, TEXT("Player changed to Team: %d"), TeamID);
}
