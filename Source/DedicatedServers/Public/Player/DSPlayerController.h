#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerController.generated.h"


UCLASS()
class DEDICATEDSERVERS_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADSPlayerController();
	virtual void ReceivedPlayer() override;
	
protected:
	UFUNCTION(Server, Reliable)
	void Server_Ping(float TimeOfRequest);
	
	UFUNCTION(Client, Reliable)
	void Client_Pong(float TimeOfRequest);
	
private:
	float SingleTripTime;
};
