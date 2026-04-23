// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DSLocalPlayerSubsystem.h"
#include "UI/Portal/Interfaces/PortalManagement.h"

void UDSLocalPlayerSubsystem::InitializeToken(const FDSAuthenticationResult& AuthResult, TScriptInterface<IPortalManagement> PortalManagement)
{
	AuthenticationResult = AuthResult;
	PortalManagerInterface = PortalManagement;
	SetRefreshTokenTimer();
}

void UDSLocalPlayerSubsystem::SetRefreshTokenTimer()
{
	UWorld* World = GetWorld();
	if (IsValid(World) && IsValid(PortalManagerInterface.GetObject()))
	{
		FTimerDelegate RefreshDelegate;
		RefreshDelegate.BindLambda([this](){PortalManagerInterface->RefreshTokens(AuthenticationResult.RefreshToken);});
		World->GetTimerManager().SetTimer(RefreshTimer, RefreshDelegate, TokenRefreshInterval, false);
	}
}

void UDSLocalPlayerSubsystem::UpdateTokens(const FString& AccessToken, const FString& IdToken)
{
	AuthenticationResult.AccessToken = AccessToken;
	AuthenticationResult.IdToken = IdToken;
	AuthenticationResult.Dump();
	SetRefreshTokenTimer();
}
