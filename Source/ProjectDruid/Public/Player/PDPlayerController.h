// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PDPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class PROJECTDRUID_API APDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APDPlayerController();
	virtual void SetupInputComponent() override;
	
protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnJumpStarted(const FInputActionValue& Value);
	void OnJumpCompleted(const FInputActionValue& Value);
	void Torch(const FInputActionValue& Value);

protected:
	/** Input Actions and Mapping Contexts */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> TorchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<UInputMappingContext*> DefaultMappingContexts;
};
