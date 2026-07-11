#pragma once

#include "CoreMinimal.h"
#include "DS_GameModeBase.h"
#include "DS_LobbyGameMode.generated.h"
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
#include "GameLiftServerSDK.h"
#endif

class UDS_GameInstanceSubsystem;

UCLASS()
class DEDICATEDSERVERS_API ADS_LobbyGameMode : public ADS_GameModeBase
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem;
	
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
	void SetServerParameters(FServerParameters& OutServerParameters);
#endif
	
	void InitGameLift();
};
