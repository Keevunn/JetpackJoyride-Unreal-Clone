// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PaperCharacter.h"
#include "JetpackPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UPaperFlipbook;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class GAME2_JETPACKJOYRIDE_API AJetpackPlayer : public APaperCharacter
{
	GENERATED_BODY()
	
public:
	AJetpackPlayer();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	
	
protected:
	// Camera ---------------------------------------------
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraArm;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	
	// Input ----------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> FlyAction;
	
	// Animation ------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UPaperFlipbook> IdleFlipbook;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UPaperFlipbook> RunFlipbook;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UPaperFlipbook> JumpFlipbook;
	
private:
	void Move(const FInputActionValue& Value);
	void Fly(const FInputActionValue& Value);
	void UpdateCamera();
	void UpdateAnimation();
	void UpdateFacing(float MovementValue);
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraLeadX = 100.f;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CorridorCentreZ = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Input|Flying")
	float GravityScale = 0.75f;
	UPROPERTY(EditAnywhere, Category = "Input|Flying")
	float JetpackAcceleration = 2.f;
	UPROPERTY(EditAnywhere, Category = "Input|Flying")
	float MaxFlySpeed = 100.f;
	UPROPERTY(EditAnywhere, Category = "Input|Flying")
	float MaxFallSpeed = -200.f;
};
