// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DSLocalPlayerSubsystem.h"
#include "UI/Portal/Interfaces/PortalManagement.h"

void UDSLocalPlayerSubsystem::Deinitialize()
{
	ClearSession();
	Super::Deinitialize();
}

void UDSLocalPlayerSubsystem::InitializeToken(const FDSAuthenticationResult& AuthResult, TScriptInterface<IPortalManagement> PortalManagement)
{
	++AuthenticationGeneration;
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
		RefreshDelegate.BindWeakLambda(this, [this]() { RequestTokenRefresh(); });
		World->GetTimerManager().SetTimer(RefreshTimer, RefreshDelegate, TokenRefreshInterval, false);
	}
}

bool UDSLocalPlayerSubsystem::RequestTokenRefresh()
{
	if (!IsValid(PortalManagerInterface.GetObject()) || AuthenticationResult.RefreshToken.IsEmpty()) return false;
	PortalManagerInterface->RefreshTokens(AuthenticationResult.RefreshToken);
	return true;
}

void UDSLocalPlayerSubsystem::ClearSession()
{
	++AuthenticationGeneration;
	if (GetWorld()) GetWorld()->GetTimerManager().ClearTimer(RefreshTimer);
	AuthenticationResult = FDSAuthenticationResult();
	PortalManagerInterface = nullptr;
	Username.Reset();
	Email.Reset();
	Password.Reset();
}

void UDSLocalPlayerSubsystem::UpdateTokens(const FString& AccessToken, const FString& IdToken)
{
	AuthenticationResult.AccessToken = AccessToken;
	AuthenticationResult.IdToken = IdToken;
	AuthenticationResult.Dump();
	SetRefreshTokenTimer();
}

FDSAuthenticationResult UDSLocalPlayerSubsystem::GetAuthResult() const
{
	return AuthenticationResult;
}
