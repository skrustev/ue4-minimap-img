// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MinimapPrivatePCH.h"
#include "MinimapEdMode.h"

#define LOCTEXT_NAMESPACE "FMinimapModule"

void FMinimapModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FMinimapEdMode>(FMinimapEdMode::EM_MinimapEdModeId, LOCTEXT("MinimapEdModeName", "MinimapEdMode"), FSlateIcon(), true);
}

void FMinimapModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FMinimapEdMode::EM_MinimapEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinimapModule, Minimap)