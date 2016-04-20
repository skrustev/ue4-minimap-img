// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FPGame.h"
#include "FPGameGameMode.h"
#include "FPGameHUD.h"
#include "FPGameCharacter.h"

AFPGameGameMode::AFPGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPGameHUD::StaticClass();
}
