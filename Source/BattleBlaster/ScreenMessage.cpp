// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessageText(FString Message)
{
	FText Messagetext = FText::FromString(Message);
	MessageBlockText->SetText(Messagetext);

}
