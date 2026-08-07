#include "UI/HTTP/HTTPRequestTypes.h"
#include "DedicatedServers/DedicatedServers.h"

namespace HTTPStatusMessages
{
	const FString SomethingWentWrong = TEXT("Something went wrong.");
}

void FDSMetaData::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("MetaData"));

	UE_LOG(LogDedicatedServers, Log, TEXT("httpStatusCode: %d"), httpStatusCode);
	UE_LOG(LogDedicatedServers, Log, TEXT("requestId: %s"), *requestId);
	UE_LOG(LogDedicatedServers, Log, TEXT("attempts: %d"), attempts);
	UE_LOG(LogDedicatedServers, Log, TEXT("totalRetryDelay: %f"), totalRetryDelay);
	
}

void FDListFleetsResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("ListFleetsResponse: "));
	for (const FString FleetId : FleetIds)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);
	}

	if (!NextToken.IsEmpty())
		UE_LOG(LogDedicatedServers, Log, TEXT("NextToken: %s"), *NextToken);

}

void FDSGameSession::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSession:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("CurrentPlayerSessionCount: %d"), CurrentPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);

	UE_LOG(LogDedicatedServers, Log, TEXT("GameProperties"));
	for (const auto& GameProperty : GameProperties)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("GameProperty: %s = %s"), *GameProperty.Key, *GameProperty.Value);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionData: %s"), *GameSessionData);
	UE_LOG(LogDedicatedServers, Log, TEXT("IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("Location: %s"), *Location);
	UE_LOG(LogDedicatedServers, Log, TEXT("MatchMakerData: %s"), *MatchMakerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("MaxPlayerSessionCount: %d"), MaxPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("Name: %s"), *Name);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSessionCreationPolicy: %s"), *PlayerSessionCreationPolicy);
	UE_LOG(LogDedicatedServers, Log, TEXT("Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("StatusReason: %s"), *StatusReason);
	UE_LOG(LogDedicatedServers, Log, TEXT("TerminationTime: %s"), *TerminationTime);
}

void FDSPlayerSession::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSession:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerData: %s"), *PlayerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerId: %s"), *PlayerId);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSessionId: %s"), *PlayerSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("TerminationTime: %s"), *TerminationTime);
}

void FDSCodeDeliveryDetails::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("CodeDeliveryDetails:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("AttributeName: %s"), *AttributeName);
	UE_LOG(LogDedicatedServers, Log, TEXT("DeliveryMedium: %s"), *DeliveryMedium);
	UE_LOG(LogDedicatedServers, Log, TEXT("Destination: %s"), *Destination);
}

void FDSSignUpResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("SignUpResponse:"));
	CodeDeliveryDetails.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %s"), *Session);
	UE_LOG(LogDedicatedServers, Log, TEXT("UserConfirmed: %s"), UserConfirmed ? TEXT("true") : TEXT("false"));
	UE_LOG(LogDedicatedServers, Log, TEXT("UserSub: %s"), *UserSub);
}

void FDSNewDeviceMetadata::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("NewDeviceMetadata:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("DeviceGroupKey: %s"), *DeviceGroupKey);
	UE_LOG(LogDedicatedServers, Log, TEXT("DeviceKey: %s"), *DeviceKey);
}

void FDSAuthenticationResult::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("AuthenticationResult:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("AccessToken: %s"), *AccessToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("ExpiresIn: %d"), ExpiresIn);
	UE_LOG(LogDedicatedServers, Log, TEXT("IdToken: %s"), *IdToken);
	NewDeviceMetadata.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("RefreshToken: %s"), *RefreshToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("TokenType: %s"), *TokenType);
}

void FDSInitiateAuthResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("InitiateAuthResponse:"));
	AuthenticationResult.Dump();

	for (const FString& Challenge : AvailableChallenges)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("AvailableChallenge: %s"), *Challenge);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("ChallengeName: %s"), *ChallengeName);

	for (const auto& Parameter : ChallengeParameters)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("ChallengeParameter: %s = %s"), *Parameter.Key, *Parameter.Value);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %s"), *Session);
	UE_LOG(LogDedicatedServers, Log, TEXT("Email: %s"), *email);
}
