#include "UI/Portal/Dashboard/LeaderboardPage.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "UI/Portal/Dashboard/LeaderboardCard.h"

void ULeaderboardPage::PopulateLeaderboard(TArray<FDSLeaderboardItem>& Leaderboard)
{
	ScrollBox_Leaderboard->ClearChildren();
	CalculateLeaderboardPlaces(Leaderboard);
	
	for (const FDSLeaderboardItem& Item : Leaderboard)
	{
		ULeaderboardCard* LeaderboardCard = CreateWidget<ULeaderboardCard>(this, LeaderboardCardClass);
		if (IsValid(LeaderboardCard))
		{
			LeaderboardCard->SetPlayerInfo(Item.username, Item.matchWins, Item.place);
			ScrollBox_Leaderboard->AddChild(LeaderboardCard);
		}
	}
}

void ULeaderboardPage::CalculateLeaderboardPlaces(TArray<FDSLeaderboardItem>& OutLeaderboard)
{
	// Sort Descending (O(N log N))
	OutLeaderboard.Sort([](const FDSLeaderboardItem& A, const FDSLeaderboardItem& B)
	{
		return A.matchWins > B.matchWins;	//Player with the most Match wins A,Player B with the second most Match wins and so on.
	});
	
	// Dense ranking (1,2,2,3)
	int32 CurrentRank = 1;
	for (int32 i = 0; i < OutLeaderboard.Num(); ++i)
	{
		// If not first time and have fewer wins than person above them then drop the rank.
		if (i > 0 && OutLeaderboard[i].matchWins < OutLeaderboard[i -1].matchWins)
		{
			CurrentRank++;
		}
		// Every player gets assigned the CurrentRank, whether they tied or not.
		OutLeaderboard[i].place = CurrentRank;
	}
}

void ULeaderboardPage::SetStatusMessage(const FString& StatusMessage, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(StatusMessage));
}
