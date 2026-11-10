// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelChunk.generated.h"

class UPaperTileSet;
class UPaperTileMap;
class UBoxComponent;
class UPaperTileMapComponent;
class UPaperSpriteComponent;

UCLASS()
class GAME2_JETPACKJOYRIDE_API ALevelChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	int		GetChunkWidth() const		{ return ChunkWidth; }
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Environment Sprites")
	TObjectPtr<UPaperSpriteComponent> BackgroundSprite;
	
	UPROPERTY(VisibleAnywhere, Category = "Environment Sprites")
	TObjectPtr<UPaperTileMapComponent> ForegroundTileMap;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Environment Sprites")
	int ChunkWidth = 96; // pixel width (and unreal units)
};
