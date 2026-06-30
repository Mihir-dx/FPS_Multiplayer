// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "GameFramework/GameMode.h"
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
#include "GameLiftServerSDK.h"
#endif
#include "DS_GameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDS_GameMode, Log, All);

/**
 * */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameMode : public ADS_GameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
	void OnStartGameSession(Aws::GameLift::Server::Model::GameSession OnGameSession);
	void OnProcessTerminate();
	bool OnHealthCheck();
#endif
    
private:
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
	TSharedPtr<FProcessParameters> ProcessParameters;
	void SetServerParameters(FServerParameters& OutServerParameters);
#endif
    
	void InitGameLift();
};