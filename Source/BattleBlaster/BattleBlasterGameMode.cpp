// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"

#include "Kismet/GameplayStatics.h"	
#include "BattleBlasterGameInstance.h"
#include "Screenmessage.h"
#include "Tower.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray <AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();
	UE_LOG(LogTemp, Display, TEXT("Tower Count: %d"), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(PlayerPawn) {
		Tank = Cast<ATank>(PlayerPawn);
		if(!Tank) {
			UE_LOG(LogTemp, Error, TEXT("Player Pawn is not a Tank!"));
		}
	}

	
	int32 LoopIndex = 0;
	while (LoopIndex < TowerCount) {
		AActor* TowerActor = Towers[LoopIndex];
		if (TowerActor) {
						ATower* Tower = Cast<ATower>(TowerActor);
			if(Tower && Tank) {
				Tower->Tank = Tank;
			}
		}
		LoopIndex++;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PlayerController) {
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController,ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Get Ready Player!");

		}
	}
	



	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountdownTimerTimeOut, CountdownDelay,1.0f , true);


	





}
void ABattleBlasterGameMode::OnCountdownTimerTimeOut()
{
	CountdownSeconds -= 1;
	

	if (CountdownSeconds > 0) {
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
	}
	else if(CountdownSeconds == 0) {
		ScreenMessageWidget->SetMessageText("Go");
		Tank->SetPlayerEnabled(true);
	}
	else {
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}



	
}


void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{

	bool IsGameOver = false;
	

	if(DeadActor == Tank) {
		Tank->HandleDestruction();
		IsGameOver = true;
		
	}
	else {
		ATower* DestroyedTower = Cast<ATower>(DeadActor);
		if(DestroyedTower) {
			DestroyedTower->HandleDestruction();
			
			TowerCount--;
			if(TowerCount==0) {
				IsGameOver = true;
				IsVictory = true;
			}
			
		}
	}
	if (IsGameOver) {
		FString GameOverString = IsVictory ? "Victory!" : "Defeat!";

		ScreenMessageWidget->SetMessageText(GameOverString);
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimerTimeOut, GameOverDelay, false);
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeOut()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance) {
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance) {
			if (IsVictory) {
				BattleBlasterGameInstance->LoadNextlevel();
			}
			else {
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}
}

