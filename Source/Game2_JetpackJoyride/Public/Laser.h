// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Laser.generated.h"

class UPaperSpriteComponent;
class UBoxComponent;

/*
	 * 3 variations of length: small (15), medium (25), large (30)
	 *                 rotation: 0 deg, +/- 45 deg, 90 deg
	 * To be defined with blueprints, positioning will be random
	 * Define Z range according to length and rotation
	 * 
	 * For all lengths: 90 deg - [-48, 48]
	 * 
	 * Small:
	 * 0 deg - [-40.5, 40.5]
	 * 45 deg - [-43, 43]
	 * 
	 * Medium:
	 * 0 deg - [-35.5, 35.5]
	 * 45 deg - [-39.5, 39.5]
	 * 
	 * Large:
	 * 0 deg - [-33, 33]
	 * 45 deg - [-37.5, 37.5]
	 */

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
		
	void SetLength(const float Length) { LaserLength = Length; UpdateLaserGeometry(); }
	float GetLength() const { return LaserLength; }
	
	void SetRotation(const float AngleDeg) { RotationY = AngleDeg; }
	float GetRotation() const { return RotationY; }
	
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
	UPROPERTY(EditAnywhere, Category = "Spawn Config", meta = (ClampMin = 0.f))
	float LaserLength = 8.f; // By default
	UPROPERTY(VisibleAnywhere, Category = "Spawn Config")
	float RotationY = 0.f; // By default
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Config|Ranges", meta = (ClampMin = 15.f, ClampMax = 30.f))
	float MinLength = 15.f;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Config|Ranges", meta = (ClampMin = 15.f, ClampMax = 30.f))
	float MaxLength = 30.f;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Config|Ranges", meta = (ClampMin = 45.f, ClampMax = 135.f))
	float MinRotation = 45.f;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Config|Ranges", meta = (ClampMin = 45.f, ClampMax = 135.f))
	float MaxRotation = 135.f;
};
