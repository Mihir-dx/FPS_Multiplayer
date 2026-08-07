#pragma once

#include "CoreMinimal.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "GameStatsManager.generated.h"


struct FDSRecordMatchStatsInput;

UCLASS()
class DEDICATEDSERVERS_API UGameStatsManager : public UHTTPRequestManager
{
	GENERATED_BODY()
	
private:
	void RecordMatchStats(const FDSRecordMatchStatsInput& RecordMatchStatsInput);
};
