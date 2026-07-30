#include "Game/DS_LobbyGameMode.h"
#include "Game/DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyState.h"
#include "Player/DSPlayerController.h"
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
	
	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
		AddPlayerInfoToLobbyState(NewController);
}

void ADS_LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	CheckAndStopLobbyCountdown();
	RemovePlayerSession(Exiting);
	
	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
		RemovePlayerInfoFromLobbyState(Exiting);
}

void ADS_LobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	
	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));
	
	TryAcceptPlayerSession(PlayerSessionId, Username, ErrorMessage);
}

FString ADS_LobbyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	FString InitializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
	
	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString Username = UGameplayStatics::ParseOption(Options, TEXT("Username"));
	
	if (ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(NewPlayerController); IsValid(DSPlayerController))
	{
		DSPlayerController->PlayerSessionId = PlayerSessionId;
		DSPlayerController->Username = Username;
	}
	
	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
		AddPlayerInfoToLobbyState(NewPlayerController);
	
	return InitializedString;
}

void ADS_LobbyGameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username,
                                               FString& OutErrorMessage)
{
	if (PlayerSessionId.IsEmpty() || Username.IsEmpty())
	{
		OutErrorMessage = TEXT("PlayerSessionId and/or Username is invalid");
		return;
	}
	
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
	Aws::GameLift::Server::Model::DescribePlayerSessionsRequest DescribePlayerSessionsRequest;
	DescribePlayerSessionsRequest.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId));
	
	const auto& DescribePlayerSessionsOutcome = Aws::GameLift::Server::DescribePlayerSessions(DescribePlayerSessionsRequest);
	if (!DescribePlayerSessionsOutcome.IsSuccess())
	{
		OutErrorMessage  = TEXT("DescribePlayerSessions Failed!");
		return;
	}
	
	const auto& DescribePlayerSessionsResult = DescribePlayerSessionsOutcome.GetResult();
	int32 Count = 0;
	const Aws::GameLift::Server::Model::PlayerSession* PlayerSessions = DescribePlayerSessionsResult.GetPlayerSessions(Count);
	if (PlayerSessions == nullptr || Count == 0)
	{
		OutErrorMessage = TEXT("GetPlayerSessions Failed!");
		return;
	}
	
	for (int32 i = 0; i < Count; i++)
	{
		const Aws::GameLift::Server::Model::PlayerSession& PlayerSession = PlayerSessions[i];
		if (Username.Equals(PlayerSession.GetPlayerId())) continue;
		if (PlayerSession.GetStatus() != Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED)
		{
			OutErrorMessage = FString::Printf(TEXT("Session for %s is not Reserved; Fail PreLogin."), *Username);
			return;
		}
		
		const auto& AcceptPlayerSessionOutcome = Aws::GameLift::Server::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
		OutErrorMessage = AcceptPlayerSessionOutcome.IsSuccess() ? "" : FString::Printf(TEXT("Failed to Accept Player sessions from %s"), *Username);
	}
	
#endif
}

void ADS_LobbyGameMode::AddPlayerInfoToLobbyState(AController* Player) const
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Player);
	ADS_GameState* DSGameState = GetGameState<ADS_GameState>();
	if (IsValid(DSGameState) && IsValid(DSGameState->LobbyState) && IsValid(DSPlayerController))
	{
		FLobbyPlayerInfo PlayerInfo(DSPlayerController->Username);
		DSGameState->LobbyState->AddPlayerInfo(PlayerInfo);
	}
}

void ADS_LobbyGameMode::RemovePlayerInfoFromLobbyState(AController* Player) const
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(Player);
	ADS_GameState* DSGameState = GetGameState<ADS_GameState>();
	if (IsValid(DSGameState) && IsValid(DSGameState->LobbyState) && IsValid(DSPlayerController))
	{
		DSGameState->LobbyState->RemovePlayerInfo(DSPlayerController->Username);
	}
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
		StopCountdownTimer(LobbyCountdownTimer);
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