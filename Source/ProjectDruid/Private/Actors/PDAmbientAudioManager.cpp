#include "Actors/PDAmbientAudioManager.h"
#include "Components/AudioComponent.h"

APDAmbientAudioManager::APDAmbientAudioManager()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AmbientMusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AmbientMusicComponent"));
	AmbientMusicComponent->bAutoActivate = false;
	RootComponent = AmbientMusicComponent;
}

void APDAmbientAudioManager::BeginPlay()
{
	Super::BeginPlay();
    
	if (AmbientMusic)
	{
		AmbientMusicComponent->SetSound(AmbientMusic);
		AmbientMusicComponent->VolumeMultiplier = AmbientMusicVolume;
		PlayAmbientMusic();
	}
}

void APDAmbientAudioManager::PlayAmbientMusic()
{
	if (AmbientMusicComponent && !AmbientMusicComponent->IsPlaying())
	{
		AmbientMusicComponent->Play();
	}
}

void APDAmbientAudioManager::StopAmbientMusic()
{
	if (AmbientMusicComponent && AmbientMusicComponent->IsPlaying())
	{
		AmbientMusicComponent->Stop();
	}
}