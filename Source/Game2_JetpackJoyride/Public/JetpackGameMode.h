// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JetpackGameMode.generated.h"

class ALaser;
class AJetpackPlayer;
class ALevelChunk;

struct FLaserSpawnSettings {
	float Length = 0.f;
	float AngleDeg = 0.f;
	float ZPosition = 0.f;
};

namespace LaserPresets {
	FLaserSpawnSettings Generate();
	
	inline constexpr float Lengths[] = {15.f, 25.f, 30.f};
	inline constexpr float Angles[] = {-45.f, 0.f, 45.f, 90.f};
	constexpr float BoundaryValue = 48.f;
}

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
	void SpawnLaser();
	
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
	
	UPROPERTY(EditDefaultsOnly, Category = "Lasers")
	TSubclassOf<ALaser> LaserClass;
	UPROPERTY(EditDefaultsOnly, Category = "Lasers")
	float ObstacleGenerationDistance = 400.f;
	UPROPERTY(EditDefaultsOnly, Category = "Lasers", meta = (ClampMin = 0.f, ClampMax = 100.f))
	int LaserSpawnProbability = 30; // percentage; TODO Increases as the player goes further until a limit
	UPROPERTY()
	TArray<TObjectPtr<ALaser>> ActiveLasers;
	float NextLaserStartX = 0;
	float LastLaserEndX = 0;
	const float MinGap = 20.f;
	const float MaxGap = 50.f;
	int MaxConsecutiveSlices = 3;
	int ConsecutiveSlicesCounter = 0;
};
