#include "UI/Lobby/PlayerLabel.h"

void UPlayerLabel::SetUsername(const FString& Username) const
{
	TextBlock_Username->SetText(FText::FromString(Username));
}

FString UPlayerLabel::GetUsername() const
{
	return TextBlock_Username->GetText().ToString();
}
