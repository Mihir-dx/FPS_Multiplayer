// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "DSLocalPlayerSubsystem.generated.h"

class IPortalManagement;

UCLASS()
class DEDICATEDSERVERS_API UDSLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Deinitialize() override;
	void InitializeToken(const FDSAuthenticationResult& AuthResult, TScriptInterface<IPortalManagement> PortalManagement);
	void SetRefreshTokenTimer();
	void UpdateTokens(const FString& AccessToken, const FString& IdToken);
	FDSAuthenticationResult GetAuthResult() const;
	bool RequestTokenRefresh();
	void ClearSession();
	uint64 GetAuthenticationGeneration() const { return AuthenticationGeneration; }
	
	FString Username{};
	FString Email{};
	FString Password{};
	
private:
	uint64 AuthenticationGeneration = 0;
	
	UPROPERTY()
	FDSAuthenticationResult AuthenticationResult;
	
	UPROPERTY()
	TScriptInterface<IPortalManagement> PortalManagerInterface;
	
	// 75% of an hour (the expiration time for AccessToken and IdToken)
	float TokenRefreshInterval = 2700.f;
	FTimerHandle RefreshTimer;
};
