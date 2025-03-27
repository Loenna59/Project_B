#pragma once

#include "BanimalsType.generated.h"

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	None,
	Red,
	Blue,
	Yellow,
	Green
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PlayerID;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETeamType Team;

	// 해당 플레이어가 준비 상태인지 여부 (예: 캐릭터 선택완료)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsReady;

	// 현재 생존 여부
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAlive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsWin;

	// 기본 생성자
	FPlayerInfo()
		: PlayerID(-1)
		, PlayerName(TEXT("Unknown"))
		, Team(ETeamType::None)
		, bIsReady(false)
		, bIsAlive(true)
		, bIsWin(false)
	{}
};

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MapID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 TeamMaxPlayers;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UUserWidget* LoadingWidget;
	
	// 기본 생성자
	FMapInfo()
		: MapID(-1)
		, TeamMaxPlayers(-1)
		, MaxPlayers(8)
		, LoadingWidget(nullptr)
	{}
};

USTRUCT()
struct FNetInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FString, FPlayerInfo> Info;
};