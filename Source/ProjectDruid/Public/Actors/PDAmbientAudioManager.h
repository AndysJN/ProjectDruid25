#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "PDAmbientAudioManager.generated.h"

UCLASS()
class PROJECTDRUID_API APDAmbientAudioManager : public AActor
{
	GENERATED_BODY()

public:
	APDAmbientAudioManager();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundBase> AmbientMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float AmbientMusicVolume = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	bool bLoopAmbientMusic = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<UAudioComponent> AmbientMusicComponent;

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayAmbientMusic();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void StopAmbientMusic();
};