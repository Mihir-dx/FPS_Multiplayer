#include "UI/Portal/Dashboard/LeaderboardPage.h"
#include "Components/ScrollBox.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "UI/Portal/Dashboard/LeaderboardCard.h"

void ULeaderboardPage::PopulateLeaderboard(TArray<FDSLeaderboardItem> Leaderboard)
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

void ULeaderboardPage::CalculateLeaderboardPlaces(TArray<FDSLeaderboardItem> OutLeaderboard)
{
	OutLeaderboard.Sort([](const FDSLeaderboardItem& A, const FDSLeaderboardItem& B)
	{
		return A.matchWins > B.matchWins;	//Player with the most Match wins A,Player B with the second most Match wins and so on.
	});
	
	// Assign place based on wins, accounting for ties.
	int32 CurrentRank = 1;
	for (int32 i = 0; i < OutLeaderboard.Num(); ++i)
	{
		if (i > 0 && OutLeaderboard[i].matchWins == OutLeaderboard[i -1].matchWins)
		{
			// If wins are same as previous, then they are on the same place.
			OutLeaderboard[i].place = OutLeaderboard[i - 1].place;
		}
		else
		{
			// If wins are different, assign current rank as the place.
			OutLeaderboard[i].place = CurrentRank;
		}
		// Increment rank for the next item.
		CurrentRank++;
	}
}
