#include "Game/DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
#include "GameLiftServerSDK.h"
#endif


UDS_GameInstanceSubsystem::UDS_GameInstanceSubsystem()
{
	bGameLiftInitialized = false;	
}

#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
void UDS_GameInstanceSubsystem::InitGameLift(const FServerParameters& ServerParams)
{
	if (bGameLiftInitialized) return;
	
	UE_LOG(LogDedicatedServers, Log, TEXT("Initializing GameLift SDK..."));
	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	
	FGameLiftGenericOutcome InitOutcome = GameLiftSdkModule->InitSDK(ServerParams);
	if (!InitOutcome.IsSuccess())
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("InitSDK Failed: %s"), *InitOutcome.GetError().m_errorMessage);
		return;
	}

	ProcessParameters = MakeShared<FProcessParameters>();
	
	ProcessParameters->OnStartGameSession.BindUObject(this, &UDS_GameInstanceSubsystem::OnStartGameSession);
	ProcessParameters->OnTerminate.BindUObject(this, &UDS_GameInstanceSubsystem::OnProcessTerminate);
	ProcessParameters->OnHealthCheck.BindUObject(this, &UDS_GameInstanceSubsystem::OnHealthCheck);

	int32 Port = 7777;
	FParse::Value(FCommandLine::Get(), TEXT("port="), Port);
	ProcessParameters->port = Port;

	TArray<FString> LogFiles;
	LogFiles.Add(FPaths::ProjectLogDir() / TEXT("Server.log"));
	ProcessParameters->logParameters = LogFiles;

	UE_LOG(LogDedicatedServers, Log, TEXT("Calling ProcessReady..."));
	FGameLiftGenericOutcome ReadyOutcome = GameLiftSdkModule->ProcessReady(*ProcessParameters);

	if (ReadyOutcome.IsSuccess())
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Process Ready Succeeded!"));
	}
	else
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("Process Ready Failed: %s"), *ReadyOutcome.GetError().m_errorMessage);
	}
	bGameLiftInitialized = true;
}
#endif

#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
void UDS_GameInstanceSubsystem::OnStartGameSession(Aws::GameLift::Server::Model::GameSession OnGameSession)
{
	FString GameSessionId = FString(OnGameSession.GetGameSessionId());
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSession Starting: %s"), *GameSessionId);

	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	GameLiftSdkModule->ActivateGameSession();
}

void UDS_GameInstanceSubsystem::OnProcessTerminate()
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Game Server Process is terminating"));

	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	GameLiftSdkModule->ProcessEnding();
    
	FGenericPlatformMisc::RequestExit(false);
}

bool UDS_GameInstanceSubsystem::OnHealthCheck()
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Performing Health Check"));
	return true;
}
#endif
