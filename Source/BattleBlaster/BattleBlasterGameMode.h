// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ScreenMessage.h"
#include "Tank.h"
#include "BattleBlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenMessage> ScreenMessageClass;

	UScreenMessage* ScreenMessageWidget;

	ATank* Tank;
	int32 TowerCount;

	bool IsVictory = false;

	void ActorDied(AActor* DeadActor);

	

	UPROPERTY(EditAnywhere)
	int32 CountdownDelay = 3;

	FTimerHandle CountdownTimerHandle;


	int32 CountdownSeconds;

	UPROPERTY(EditDefaultsOnly)
	float GameOverDelay = 3.0f;

	void OnGameOverTimerTimeOut();
	void OnCountdownTimerTimeOut();

};
