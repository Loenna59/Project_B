// Fill out your copyright notice in the Description page of Project Settings.

// 블랙홀 맵 게임 전반적인 규칙관리
// 게임 시작 및 초기 설정 (인원제한, 인원배치위치, 팀 자동배정)
// 게임 진행 관리 (제한시간 설정, 종료 조건확인)
// 승리/패배 팀 결정 및 최종 결과 전달

#include "Project_B/Maps/BlackHole/Public/BlackholeGameMode.h"

#include "Project_B/Maps/BlackHole/Public/BlackholeGameState.h"
#include "Project_B/Maps/BlackHole/Public/BlackholePlayerState.h"

void ABlackholeGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 3분후에는 게임 종료 함수가 호출된다
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ABlackholeGameMode::EndGame, 180.f, false);	
}

void ABlackholeGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	// Legacy: 8명 넘으면 함수 나가자
	int32 MaxPlayers = 8;
	if (playerIdx > MaxPlayers) return;

	// 플레이어와 스테이트 가져오자
	AActor* player = Cast<AActor>(NewPlayer->GetPawn());
	ABlackholePlayerState* ps = NewPlayer->GetPlayerState<ABlackholePlayerState>();
	
	// 원 중심 기준 플레이어 배치하기
	FVector CenterPos = FVector(0,0,450);
	float Radius = 800.f;
	float Angle = (2 * PI / MaxPlayers) * playerIdx;
	FVector NewPos = CenterPos + FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f) * Radius;
	player->SetActorLocation(NewPos);

	// 로비로!
	// 들어온 순서대로 팀을 배정받자
	if (ps)
	{
		// 2명씩 팀을 자동 배정한다
		// 로비에서 맵인식해서 2,4 구분
		int TeamIndex = playerIdx % 4; // 레드(0), 블루(1), 옐로(2), 그린(3)
		ps->SetTeam(TeamIndex); // 팀 정보 설정
	}
	playerIdx++;
}

void ABlackholeGameMode::EndGame()
{
	// 게임이 종료되면, 게임 스테이트에 전달하자
	ABlackholeGameState* gs = GetGameState<ABlackholeGameState>();
	if (gs)
	{
		gs->SetGameOver();
	}
}
