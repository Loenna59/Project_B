#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(BAnimals, Log, All);

#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PRINT_CALLINFO() \
UE_LOG(BAnimals, Warning, TEXT("%s"), *CALLINFO)

/**로그에 출력, LOG_PRINT(TEXT(" "))*/
#define LOG_PRINT(Format, ...) \
UE_LOG(BAnimals, Warning, Format, ##__VA_ARGS__);

/**화면에 출력, 시간과 색상 설정 필요*/
#define LOG_SCREEN_MY(Time, Color, Format, ...) \
if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Time, Color, FString::Printf(TEXT(Format), ##__VA_ARGS__));

/**화면에 출력 LOG_SCREEN(" ")*/
#define LOG_SCREEN(Format, ...) \
LOG_SCREEN_MY(2.0f, FColor::Green, Format,  ##__VA_ARGS__)

#define LOG_SCREEN_OBJ(Obj, Format, ...) \
if (GEngine && Obj) \
{ \
FString Msg = FString::Printf(TEXT("[%s] " Format), *Obj->GetName(), ##__VA_ARGS__); \
GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, Msg); \
}

/**화면과 로그 모두에 출력, LOG_ERROR(this, TEXT(" "))*/
#define LOG_ERROR(Obj, Format, ...) { \
const FString _ObjName = (Obj) ? (Obj)->GetName() : TEXT("NULL"); \
UE_LOG(BAnimals, Error, TEXT("[%s] ") Format, *_ObjName, ##__VA_ARGS__); \
if (GEngine) { \
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("[%s] ") Format, *_ObjName, ##__VA_ARGS__)); \
} \
}