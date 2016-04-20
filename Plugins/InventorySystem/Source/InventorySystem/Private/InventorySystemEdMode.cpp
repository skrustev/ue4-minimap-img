// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "InventorySystemPrivatePCH.h"
#include "InventorySystemEdMode.h"
#include "InventorySystemEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"

const FEditorModeID FInventorySystemEdMode::EM_InventorySystemEdModeId = TEXT("EM_InventorySystemEdMode");

FInventorySystemEdMode::FInventorySystemEdMode()
{

}

FInventorySystemEdMode::~FInventorySystemEdMode()
{

}

void FInventorySystemEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FInventorySystemEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FInventorySystemEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FInventorySystemEdMode::UsesToolkits() const
{
	return true;
}




