// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MinimapPrivatePCH.h"
#include "MinimapEdMode.h"
#include "MinimapEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"

const FEditorModeID FMinimapEdMode::EM_MinimapEdModeId = TEXT("EM_MinimapEdMode");

FMinimapEdMode::FMinimapEdMode()
{

}

FMinimapEdMode::~FMinimapEdMode()
{

}

void FMinimapEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FMinimapEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FMinimapEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FMinimapEdMode::UsesToolkits() const
{
	return true;
}




