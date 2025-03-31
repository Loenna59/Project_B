// Fill out your copyright notice in the Description page of Project Settings.


#include "PodiumGameState.h"

#include "PodiumCamera.h"
#include "WinnerPrize.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Project_B/Maps/LobbyMap/BanimalsGameInstance.h"
#include "Project_B/Utilities/LogMacro.h"

class UBanimalsGameInstance;

APodiumGameState::APodiumGameState()
{
}

void APodiumGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APodiumGameState, PodiumCamera);
}

void APodiumGameState::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		FTimerHandle OnStartTimerHandle;
		
		GetWorld()->GetTimerManager().SetTimer(OnStartTimerHandle, this, &APodiumGameState::Net_Shoot,ReadyTime,false);
		InitSpawnPoints();
		
		PodiumCamera = Cast<APodiumCamera>(UGameplayStatics::GetActorOfClass(GetWorld(),APodiumCamera::StaticClass()));
	}

	UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	TArray<FString> WinnerKeys = gi->WinnerKeys;
	
	for (int i = 0; i<WinnerKeys.Num(); i++)
	{
		LOG_PRINT(TEXT("승리자 %d번 키: %s"), i, *WinnerKeys[i]);
	}
}

void APodiumGameState::InitPlayer(APlayerController* pc, bool bIsWin)
{
	pc->SetViewTarget(PodiumCamera);
	
	APawn* pawn = pc->GetPawn();

	if (bIsWin && WinIndex < 2)
	{
		pawn->SetActorLocation(WinnerPoints1[WinIndex]->GetActorLocation());
		pawn->SetActorRotation(WinnerPoints1[WinIndex]->GetActorRotation());
		WinIndex++;
		
		AddWinPrize(pawn);
	}
	else if (bIsWin && WinIndex >= 2)
	{
		pawn->SetActorLocation(WinnerPoints2[WinIndex-2]->GetActorLocation());
		pawn->SetActorRotation(WinnerPoints2[WinIndex-2]->GetActorRotation());
		WinIndex++;
		
		AddWinPrize(pawn);
	}
	else if (bIsWin == false && NorIndex < 2)
	{
		pawn->SetActorLocation(NormalPoints1[NorIndex]->GetActorLocation());
		pawn->SetActorRotation(NormalPoints1[NorIndex]->GetActorRotation());
		NorIndex++;
	}
	else if (bIsWin == false && NorIndex >= 2)
	{
		pawn->SetActorLocation(NormalPoints2[NorIndex-2]->GetActorLocation());
		pawn->SetActorRotation(NormalPoints2[NorIndex-2]->GetActorRotation());
		NorIndex++;
	}
}

void APodiumGameState::InitPlayerInfo()
{
	UBanimalsGameInstance* gi = Cast<UBanimalsGameInstance>(GetWorld()->GetGameInstance());
	
	if (isDummyPlayer)
	{
		PlayersInfo = DummyPlayersInfo();
		mykey = FString::FromInt(dummyKey);
		UE_LOG(LogTemp,Warning,TEXT("나의 키: %s"), *FString::FromInt(dummyKey));
	}
	else
	{
		PlayersInfo= gi->GetPlayerInfo();
		
		FTimerHandle LambdaTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(LambdaTimerHandle, [this]()
		{
			const FUniqueNetIdRepl& NetIdRepl = GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>()->GetUniqueId();
			
			if (NetIdRepl.IsValid())
			{
				TSharedPtr<const FUniqueNetId> NetId = NetIdRepl.GetUniqueNetId();
				mykey = NetId->ToString();
				UE_LOG(LogTemp, Error, TEXT("나의 키: %s"), *mykey);
				
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("NetIdRepl is invalid"));
			}
		}, 0.8f, false);
	}
}

void APodiumGameState::InitSpawnPoints()
{
	TArray<AActor*> actors;
	
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WinnerSpawnPoint1"), actors);

	for (AActor* actor : actors)
	{
		WinnerPoints1.Add(actor);
	}

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WinnerSpawnPoint2"), actors);

	for (AActor* actor : actors)
	{
		WinnerPoints2.Add(actor);
	}

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint1"), actors);

	for (AActor* actor : actors)
	{
		NormalPoints1.Add(actor);
	}

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint2"), actors);

	for (AActor* actor : actors)
	{
		NormalPoints2.Add(actor);
	}
}

void APodiumGameState::AddWinPrize(APawn* pawn)
{
	AWinnerPrize* prize = GetWorld()->SpawnActor<AWinnerPrize>(WinnerPrizeClass, pawn->GetTransform());
	
	if (prize)
	{
		prize->AttachToComponent(pawn->GetController()->GetCharacter()->GetMesh(), 
								 FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void APodiumGameState::Net_Shoot_Implementation()
{
	if (PodiumCamera)
	{
		PodiumCamera->Shoot();
	}
	else
	{
		LOG_ERROR(this, TEXT("포디엄 카메라 is Null"));
	}
}

TMap<FString, FPlayerInfo> APodiumGameState::DummyPlayersInfo()
{
	TMap<FString, FPlayerInfo> dummyPlayers;
	
	FPlayerInfo Player0;
	Player0.PlayerID = 0;
	Player0.PlayerName = TEXT("Player0");
	Player0.Team = ETeamType::Blue; 
	Player0.bIsReady = true;
	Player0.bIsAlive = true;
	Player0.bIsWin = true;
	dummyPlayers.Add(TEXT("0"), Player0);

	FPlayerInfo Player1;
	Player1.PlayerID = 1;
	Player1.PlayerName = TEXT("Player1");
	Player1.Team = ETeamType::Red;
	Player1.bIsReady = true;
	Player1.bIsAlive = true;
	Player1.bIsWin = true;
	dummyPlayers.Add(TEXT("1"), Player1);

	FPlayerInfo Player2;
	Player2.PlayerID = 2;
	Player2.PlayerName = TEXT("Player3");
	Player2.Team = ETeamType::Blue;
	Player2.bIsReady = true;
	Player2.bIsAlive = true;
	Player2.bIsWin = false;
	dummyPlayers.Add(TEXT("2"), Player2);
	
	return dummyPlayers;
}
