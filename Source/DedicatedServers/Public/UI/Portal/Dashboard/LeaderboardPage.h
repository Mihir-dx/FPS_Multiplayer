#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeaderboardPage.generated.h"

struct FDSLeaderboardItem;
class ULeaderboardCard;
class UTextBlock;
class UScrollBox;

UCLASS()
class DEDICATEDSERVERS_API ULeaderboardPage : public UUserWidget
{
	GENERATED_BODY()
	
public:
	 
	void PopulateLeaderboard(TArray<FDSLeaderboardItem> Leaderboard);
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Leaderboard;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;
	
	TSubclassOf<ULeaderboardCard> LeaderboardCardClass;
	
private:
	
	void CalculateLeaderboardPlaces(TArray<FDSLeaderboardItem> OutLeaderboard);
};
