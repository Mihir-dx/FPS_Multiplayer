#include "Game/DS_LobbyGameMode.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Kismet/GameplayStatics.h"
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
#include "GameLiftServerSDK.h"
#endif


ADS_LobbyGameMode::ADS_LobbyGameMode()
{
	bUseSeamlessTravel = true;
	LobbyStatus = ELobbyStatus::WaitingForPlayers;
	MinPlayers = 1;			//To be changed for actual total no. of players
	LobbyCountdownTimer.Type = ECountdownTimerType::LobbyCountdown;
}

void ADS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	CheckAndStartLobbyCountdown();
}

void ADS_LobbyGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);
	CheckAndStartLobbyCountdown();
}

void ADS_LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	CheckAndStopLobbyCountdown();
}

void ADS_LobbyGameMode::CheckAndStartLobbyCountdown()
{
	if (GetNumPlayers() >= MinPlayers && LobbyStatus == ELobbyStatus::WaitingForPlayers)
	{
		LobbyStatus = ELobbyStatus::CountdownToSeamlessTravel;
		StartCountdownTimer(LobbyCountdownTimer);
	}
}

void ADS_LobbyGameMode::CheckAndStopLobbyCountdown()
{
	if (GetNumPlayers() - 1 < MinPlayers && LobbyStatus == ELobbyStatus::CountdownToSeamlessTravel)
	{
		//If player leaves the game and goes to lobby
		//-1 indicates that player itself is leaving the game, and we need to check if the remaining players are less than the minimum required players
		LobbyStatus = ELobbyStatus::WaitingForPlayers;
		StopCountdownTimer(LobbyCountdownTimer);
	}
}

void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	InitGameLift();
}

void ADS_LobbyGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	Super::OnCountdownTimerFinished(Type);
	
	if (Type == ECountdownTimerType::LobbyCountdown)
	{
		LobbyStatus = ELobbyStatus::SeamlessTravelling;
		TrySeamlessTravel(DestinationMap);
	}
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