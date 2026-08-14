#pragma once

#include "CoreMinimal.h"
#include "UI/Portal/Dashboard/CareerPage.h"
#include "ShooterCareerPage.generated.h"

class USpecialElimData;
struct FDSRetrieveMatchStatsResponse;

UCLASS()
class FPSTEMPLATE_API UShooterCareerPage : public UCareerPage
{
	GENERATED_BODY()
	
public:
	virtual void OnRetrieveMatchStats(const FDSRetrieveMatchStatsResponse& RetrieveMatchStatsResponse) override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpecialElimData> SpecialElimDataClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCareerAchievement> CareerAchievementClass;
};
