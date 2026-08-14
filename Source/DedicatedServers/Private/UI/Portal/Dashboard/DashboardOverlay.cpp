#include "UI/Portal/Dashboard/DashboardOverlay.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/GameStats/GameStatsManager.h"
#include "UI/Portal/Dashboard/CareerPage.h"
#include "UI/Portal/Dashboard/GamePage.h"
#include "UI/Portal/Dashboard/LeaderboardPage.h"

void UDashboardOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
	GameStatsManager->OnRetrieveMatchStatsResponseReceived.AddDynamic(CareerPage, &UCareerPage::OnRetrieveMatchStats);
	GameStatsManager->RetrieveMatchStatusMessage.AddDynamic(CareerPage, &UCareerPage::SetStatusMessage);
	
	GameButton->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowGamePage);
	CareerButton->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowCareerPage);
	LeaderboardButton->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowLeaderboardPage);
	
	ShowGamePage();
}

void UDashboardOverlay::ShowGamePage()
{
	DisableButton(GameButton);
	WidgetSwitcher->SetActiveWidget(GamePage);
}

void UDashboardOverlay::ShowCareerPage()
{
	DisableButton(CareerButton);
	WidgetSwitcher->SetActiveWidget(CareerPage);
	GameStatsManager->RetrieveMatchStats();
}

void UDashboardOverlay::ShowLeaderboardPage()
{
	DisableButton(LeaderboardButton);
	WidgetSwitcher->SetActiveWidget(LeaderboardPage);
}

void UDashboardOverlay::DisableButton(UButton* Button) const
{
	GameButton->SetIsEnabled(true);
	CareerButton->SetIsEnabled(true);
	LeaderboardButton->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}
