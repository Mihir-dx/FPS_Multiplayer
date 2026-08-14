#include "UI/Career/ShooterCareerPage.h"
#include "Components/ScrollBox.h"
#include "Player/MatchPlayerState.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "UI/Portal/Dashboard/CareerAchievement.h"

void UShooterCareerPage::OnRetrieveMatchStats(const FDSRetrieveMatchStatsResponse& RetrieveMatchStatsResponse)
{
	Super::OnRetrieveMatchStats(RetrieveMatchStatsResponse);
	
	ScrollBox_Achievements->ClearChildren();
	
	TMap<ESpecialElimType, int32> AchievementsData;
	if (RetrieveMatchStatsResponse.hits > 0) AchievementsData.Emplace(ESpecialElimType::Hits, RetrieveMatchStatsResponse.hits);
	if (RetrieveMatchStatsResponse.misses > 0) AchievementsData.Emplace(ESpecialElimType::Misses, RetrieveMatchStatsResponse.misses);
	if (RetrieveMatchStatsResponse.defeats > 0) AchievementsData.Emplace(ESpecialElimType::Defeats, RetrieveMatchStatsResponse.defeats);
	if (RetrieveMatchStatsResponse.scoredElims > 0) AchievementsData.Emplace(ESpecialElimType::ScoredElims, RetrieveMatchStatsResponse.scoredElims);
	if (RetrieveMatchStatsResponse.highestStreak > 0) AchievementsData.Emplace(ESpecialElimType::Streak, RetrieveMatchStatsResponse.highestStreak);
	if (RetrieveMatchStatsResponse.dethroneElims > 0) AchievementsData.Emplace(ESpecialElimType::Dethrone, RetrieveMatchStatsResponse.dethroneElims);
	if (RetrieveMatchStatsResponse.gotFirstBlood > 0) AchievementsData.Emplace(ESpecialElimType::FirstBlood, RetrieveMatchStatsResponse.gotFirstBlood);
	if (RetrieveMatchStatsResponse.revengeElims > 0) AchievementsData.Emplace(ESpecialElimType::Revenge, RetrieveMatchStatsResponse.revengeElims);
	if (RetrieveMatchStatsResponse.showstopperElims > 0) AchievementsData.Emplace(ESpecialElimType::Showstopper, RetrieveMatchStatsResponse.showstopperElims);
	if (RetrieveMatchStatsResponse.headShotElims > 0) AchievementsData.Emplace(ESpecialElimType::Headshot, RetrieveMatchStatsResponse.headShotElims);
	
	check(SpecialElimDataClass)
	
	for (const TPair<ESpecialElimType, int32>& Pair : AchievementsData)
	{
		const FString& CareerAchievementName = SpecialElimDataClass->SpecialElimInfo.FindChecked(Pair.Key).CareerPageAchievementName;
		UTexture2D* Icon = SpecialElimDataClass->SpecialElimInfo.FindChecked(Pair.Key).ElimIcon;
		
		UCareerAchievement* CareerAchievement = CreateWidget<UCareerAchievement>(this, CareerAchievementClass);
		if (IsValid(CareerAchievement))
		{
			CareerAchievement->SetAchievementText(CareerAchievementName, Pair.Value);
			if (Icon)
				CareerAchievement->SetAchievementIcon(Icon);
			
			ScrollBox_Achievements->AddChild(CareerAchievement);
		}
	}
}
