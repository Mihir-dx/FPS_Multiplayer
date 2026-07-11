#include "Game/DS_LobbyGameMode.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
#include "GameLiftServerSDK.h"
#endif


void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	InitGameLift();
}

void ADS_LobbyGameMode::InitGameLift()
{
	#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
	if (UGameInstance* GameInstance = GetGameInstance(); IsValid(GameInstance))
	{
		if (DSGameInstanceSubsystem = GameInstance->GetSubsystem<UDS_GameInstanceSubsystem>(); IsValid(DSGameInstanceSubsystem))
		{
			FServerParameters ServerParameters;
			SetServerParameters(ServerParameters);
			DSGameInstanceSubsystem->InitGameLift(ServerParameters);
		}
	}
#endif
}

#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
void ADS_LobbyGameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	bool bIsAnywhereActive = FParse::Param(FCommandLine::Get(), TEXT("glAnywhere"));

	if (bIsAnywhereActive)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Configuring server parameters for Anywhere..."));

		FString ArgValue;

		if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereWebSocketUrl="), ArgValue))
			OutServerParameters.m_webSocketUrl = TCHAR_TO_UTF8(*ArgValue);

		if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereFleetId="), ArgValue))
			OutServerParameters.m_fleetId = TCHAR_TO_UTF8(*ArgValue);

		if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereHostId="), ArgValue))
			OutServerParameters.m_hostId = TCHAR_TO_UTF8(*ArgValue);

		if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereAuthToken="), ArgValue))
			OutServerParameters.m_authToken = TCHAR_TO_UTF8(*ArgValue);

		if (FParse::Value(FCommandLine::Get(), TEXT("glAnywhereProcessId="), ArgValue))
		{
			OutServerParameters.m_processId = TCHAR_TO_UTF8(*ArgValue);
		}
		else
		{
			FString PIDString = FString::Printf(TEXT("PID_%d"), FPlatformProcess::GetCurrentProcessId());
			OutServerParameters.m_processId = TCHAR_TO_UTF8(*PIDString);
		}
	}
	else
	{
		FString PIDString = FString::Printf(TEXT("PID_%d"), FPlatformProcess::GetCurrentProcessId());
		OutServerParameters.m_processId = TCHAR_TO_UTF8(*PIDString);
	}
}

#endif