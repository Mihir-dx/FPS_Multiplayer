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
	
public:
	ADS_LobbyGameMode();
	void CheckAndStartLobbyCountdown();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	void CheckAndStopLobbyCountdown();
	virtual void Logout(AController* Exiting) override;
	
	UPROPERTY()
	ELobbyStatus LobbyStatus;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MinPlayers;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DestinationMap;	//Pointer To Map's Address
	
private:
	
	UPROPERTY()
	TObjectPtr<UDS_GameInstanceSubsystem> DSGameInstanceSubsystem;
	
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle LobbyCountdownTimer;
	
#if defined(WITH_GAMELIFT) && WITH_GAMELIFT
	void SetServerParameters(FServerParameters& OutServerParameters);
#endif
	
	void InitGameLift();
};
