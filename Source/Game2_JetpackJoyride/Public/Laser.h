// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Laser.generated.h"

class UPaperSpriteComponent;
class UBoxComponent;

UCLASS()
class GAME2_JETPACKJOYRIDE_API ALaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	void UpdateLaserGeometry();
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> Root;
	UPROPERTY()
	TObjectPtr<USceneComponent> SpriteRoot;
	
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	TObjectPtr<UBoxComponent> CollisionBox;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sprites")
	TObjectPtr<UPaperSpriteComponent> TopSprite;
	UPROPERTY(EditDefaultsOnly, Category = "Sprites")
	TObjectPtr<UPaperSpriteComponent> BottomSprite;
	UPROPERTY(EditDefaultsOnly, Category = "Sprites")
	TObjectPtr<UPaperSpriteComponent> MidSprite; // Sprite: 8x8; scaled by 0.75 along x to fit end sprites 
	
	const float OriginalLength = 8.f;
	UPROPERTY(EditAnywhere, Category = "Spawn Config");
	float LaserLength = 8.f; // By default
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Config|Ranges", meta = (ClampMin = 48.f, ClampMax = 80.f))
	float MinLength = 48.f;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Config|Ranges", meta = (ClampMin = 48.f, ClampMax = 80.f))
	float MaxLength = 80.f;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Config|Ranges", meta = (ClampMin = 45.f, ClampMax = 135.f))
	float MinRotation = 45.f;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Config|Ranges", meta = (ClampMin = 45.f, ClampMax = 135.f))
	float MaxRotation = 135.f;
};
