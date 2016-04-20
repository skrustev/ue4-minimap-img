// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "InventorySystemPrivatePCH.h"
#include "InventorySystemEdMode.h"

#define LOCTEXT_NAMESPACE "FInventorySystemModule"

void FInventorySystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FInventorySystemEdMode>(FInventorySystemEdMode::EM_InventorySystemEdModeId, LOCTEXT("InventorySystemEdModeName", "InventorySystemEdMode"), FSlateIcon(), true);
}

void FInventorySystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FInventorySystemEdMode::EM_InventorySystemEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInventorySystemModule, InventorySystem)