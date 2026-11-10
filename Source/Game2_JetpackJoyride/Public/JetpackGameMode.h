// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JetpackGameMode.generated.h"

class AJetpackPlayer;
class ALevelChunk;
/**
 * 
 */
UCLASS()
class GAME2_JETPACKJOYRIDE_API AJetpackGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AJetpackGameMode();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void SpawnNextChunk();
	
	float GetVisibleWidth() const { return VisibleWidth; }
	float GetVisibleHeight() const { return VisibleHeight; }
	
protected:
	const float AspectRatio = 16.0f / 9.0f;
	const int LargestChunkWidth = 96;
	UPROPERTY()
	TObjectPtr<AJetpackPlayer> Player;
	
	float CorridorEndX = 312.f; // taken from editor, last manually placed chunk
	
	UPROPERTY(EditDefaultsOnly, Category = "Game Properties")
	float VisibleHeight = 176; // Unreal units; width = height * 16/9; full corridor height
	UPROPERTY(VisibleDefaultsOnly, Category = "Game Properties")
	float VisibleWidth = VisibleHeight * AspectRatio; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Infinite Corridor")
	float ForwardGenerationDistance;
	UPROPERTY(EditDefaultsOnly, Category = "Infinite Corridor")
	float BackGenerationDistance = LargestChunkWidth;
	
	UPROPERTY(EditDefaultsOnly, Category = "Level Chunks")
	TArray<TSubclassOf<ALevelChunk>> LevelChunkPool;
	UPROPERTY()
	TArray<TObjectPtr<ALevelChunk>> ActiveChunks;
};
