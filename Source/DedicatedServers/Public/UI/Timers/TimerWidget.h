#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/DSTypes.h"
#include "TimerWidget.generated.h"

class UTextBlock;
class ADSPlayerController;

UCLASS()
class DEDICATEDSERVERS_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Time;
	
	float InternalCountdown;
	
	void TimerStarted(float InitialTime);
	void TimerStopped();
	void UpdateCountdown(float TimeSeconds);
	
public:
	
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ECountdownTimerType TimerType = ECountdownTimerType::None;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ADSPlayerController> OwningPlayerController;
	
	UFUNCTION()
	virtual void OnTimerUpdated(float CountdownTimeLeft, ECountdownTimerType Type);
	
	UFUNCTION()
	virtual void OnTimerStopped(float CountdownTimeLeft, ECountdownTimerType Type);
	
	//To be implemented in Blueprints 
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Timer Updated"))
	void K2_OnTimerUpdated(float Time, ECountdownTimerType Type);		//K2 is used to indicate Blueprint implementation for a C++ function.
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Timer Stopped"))
	void K2_OnTimerStopped(float Time, ECountdownTimerType Type);		//K2 is used to indicate Blueprint implementation for a C++ function.
	
	UPROPERTY(BlueprintReadOnly)
	bool bActive = false;
	
	UPROPERTY(EditDefaultsOnly)
	bool bCanBeNegative = false;
	
	UPROPERTY(EditDefaultsOnly)
	bool bShowCentiSeconds = true;
	
	UPROPERTY(EditDefaultsOnly)
	bool bHiddenWhenInactive = true;
	
	FString FormatTimeAsString(float TimeSeconds) const;
};
