#pragma once

#include "CoreMinimal.h"
#include "Types/DSTypes.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimerStateChangedDelegate, float, Time, ECountdownTimerType, Type);

UCLASS()
class DEDICATEDSERVERS_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADSPlayerController();
	virtual void ReceivedPlayer() override;
	
	UFUNCTION(Client, Reliable)
	void Client_TimerUpdated(float CountdownTimeLeft, ECountdownTimerType Type) const;
	
	UFUNCTION(Client, Reliable)
	void Client_TimerStopped(float CountdownTimeLeft, ECountdownTimerType Type) const;
	
	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangedDelegate OnTimerUpdated;
	
	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChangedDelegate OnTimerStopped;
	
protected:
	UFUNCTION(Server, Reliable)				//RPC
	void Server_Ping(float TimeOfRequest);
	
	UFUNCTION(Client, Reliable)				//RPC
	void Client_Pong(float TimeOfRequest);
	
private:
	float SingleTripTime;
};
