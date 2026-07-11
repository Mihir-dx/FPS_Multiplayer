#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DS_GameInstanceSubsystem.generated.h"
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
#include "GameLiftServerSDK.h"
#endif

UCLASS()
class DEDICATEDSERVERS_API UDS_GameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UDS_GameInstanceSubsystem();
	
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
	void InitGameLift(const FServerParameters& ServerParams);
#endif
	
	UPROPERTY(BlueprintReadOnly)
	bool bGameLiftInitialized;
	
private:
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
	TSharedPtr<FProcessParameters> ProcessParameters;
#endif
	
protected:
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
	void OnStartGameSession(Aws::GameLift::Server::Model::GameSession OnGameSession);
	void OnProcessTerminate();
	bool OnHealthCheck();
#endif
};
