#include "MinimapPrivatePCH.h"
#include "MinimapWidget.h"
#include "CanvasPanel.h"
#include "CanvasPanelSlot.h"
#include "Image.h"
#include "WidgetTree.h"

UMinimapWidget::UMinimapWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ActorWidgetOb(TEXT("/Minimap/PlayerWidget"));
	ActorWidgetClass = ActorWidgetOb.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> WaypointWidgetOb(TEXT("/Minimap/WaypointWidget"));
	WaypointWidgetClass = WaypointWidgetOb.Class;

	Actors.Empty();
	ActorsWidgets.Empty();
}

UUserWidget* UMinimapWidget::CreateActorWidget(FLinearColor InColor, UPanelWidget* MinimapPanel, FName IconName)
{
	UUserWidget* NewActorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), ActorWidgetClass);
	MinimapPanel->AddChild(NewActorWidget);

	UImage* ActorIcon = Cast<UImage>(NewActorWidget->GetWidgetFromName(IconName));
	if (ActorIcon)
	{
		//Set color of the icon
		FSlateBrush NewColor;
		NewColor.TintColor = FSlateColor(InColor);
		ActorIcon->SetBrush(NewColor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Failed to find widget actor icon!!!")));
	}

	UCanvasPanelSlot* ActorPanelSlot = Cast<UCanvasPanelSlot>(NewActorWidget->Slot);
	if (ActorPanelSlot)
	{
		ActorPanelSlot->SetAnchors(FAnchors(0.5, 0.5, 0.5, 0.5));
		ActorPanelSlot->SetAlignment(FVector2D(0.5, 0.5));
		ActorPanelSlot->SetPosition(FVector2D(0, 0));
		ActorPanelSlot->SetSize(FVector2D(0.5, 0.5));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Failed to add actor!!!")));
	}
		
	return NewActorWidget;
}

void UMinimapWidget::AddActors(TArray<AActor*> TargetActors, FLinearColor InColor, UPanelWidget* MinimapPanel, FName IconName)
{
	if (Actors.Num() != ActorsWidgets.Num())
	{
		Actors.Empty();
		ActorsWidgets.Empty();
	}
	else
	{
		for (int ActorIndex = 0; ActorIndex < TargetActors.Num(); ActorIndex++)
		{
			if (TargetActors[ActorIndex])
			{
				UUserWidget* NewActorWidget = CreateActorWidget(InColor, MinimapPanel, IconName);
				if (NewActorWidget)
				{
					Actors.Add(TargetActors[ActorIndex]);
					ActorsWidgets.Add(NewActorWidget);
				}
			}
		}
	}
}

void UMinimapWidget::UpdateActor(UUserWidget* TargetActorWidget, APawn* TargetPawn, float DefaultRadius)
{
	FVector DirVector = GetOwningPlayer()->GetPawn()->GetActorLocation() - TargetPawn->GetController()->GetPawn()->GetActorLocation();
	FVector2D ScaledDir2D = FVector2D(DirVector.X / MapSize.X, DirVector.Y / MapSize.Y);
	ScaledDir2D = ScaledDir2D * 10.f * MinimapScale;

	FVector Relative3D = FVector(ScaledDir2D.X, ScaledDir2D.Y, 0.f);
	FRotator PlayerRotation = GetOwningPlayer()->GetControlRotation();
	FRotator Rotator3D = FRotator(-PlayerRotation.Pitch, -(PlayerRotation.Yaw - 90.f), -PlayerRotation.Roll);
	Relative3D = Rotator3D.RotateVector(Relative3D);

	if (!bIsCircle)
	{
		TargetActorWidget->SetVisibility(ESlateVisibility::Visible);
		UCanvasPanelSlot* TargetActorSlot = Cast<UCanvasPanelSlot>(TargetActorWidget->Slot);
		TargetActorSlot->SetPosition(FVector2D(Relative3D.X, Relative3D.Y));
	}
	else
	{
		float SquaredXY = FMath::Pow(Relative3D.X, 2) + FMath::Pow(Relative3D.Y, 2);
		float SquaredRadius = FMath::Pow(DefaultRadius, 2);
		if (SquaredRadius > SquaredXY)
		{
			TargetActorWidget->SetVisibility(ESlateVisibility::Visible);
			UCanvasPanelSlot* TargetActorSlot = Cast<UCanvasPanelSlot>(TargetActorWidget->Slot);
			TargetActorSlot->SetPosition(FVector2D(Relative3D.X, Relative3D.Y));
		}
		else
		{
			TargetActorWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

FVector2D UMinimapWidget::GetCircleClampedCoords(FVector2D InCoords, float InRadius)
{
	/**
	* Based on the equations for line and circle we intersect them and that way we get the clamped coordinates
	* Line: y = m*x + c;
	* Circle: (x − p)^2 + (y − q)^2 = r^2
	* We take y and put it in the equation of the circle and we get:
	*  (m^2 + 1)*x^2 + 2*(m*c − m*q − p)*x + (q^2 − r^2 + p^2 − 2*c*q + c^2) = 0
	*
	* Finally we only need to calculate:
	* m = y / x
	* A = m^2 + 1
	* C = - r^2
	*/
	float A, C, m;
	m = InCoords.Y / InCoords.X;
	A = m * m + 1;
	C = -(InRadius * InRadius);

	float outX = FMath::Sqrt(-4 * A * C) / (2 * A);
	float outY = m * outX;


	if (InCoords.X > 0)
	{
		return FVector2D(outX, outY);
	}
	else
	{
		return FVector2D(-outX, -outY);
	}
}

float UMinimapWidget::GetWaypointIconAngle(FVector2D NewPos)
{
	float X = NewPos.X;
	float Y = NewPos.Y;
	float SinXHyp = X / FMath::Sqrt(X * X + Y * Y);
	float Angle = (180.f) / PI * FMath::Asin(SinXHyp);

	if (Y < 0.f)
	{
		//Above the center (because in UI Y is reversed)
		return Angle;
	}
	else
	{
		//Bellow the center
		return FMath::Sign(X) * 90.f + (FMath::Sign(X) * 90.f - Angle);
	}
}

void UMinimapWidget::SetWaypointIconOutside(UImage* WaypointIcon, UTexture2D* Texture, FVector2D NewPos)
{
	//Set icon texture
	WaypointIcon->SetBrushFromTexture(Texture);

	//Set icon rotation
	float NewAngle = GetWaypointIconAngle(NewPos);
	WaypointIcon->SetRenderAngle(NewAngle);
}

void UMinimapWidget::SetWaypointIconInside(UImage* WaypointIcon, UTexture2D* Texture)
{
	//Set icon texture
	WaypointIcon->SetBrushFromTexture(Texture);

	//Set icon rotation
	WaypointIcon->SetRenderAngle(0.f);
}

void UMinimapWidget::UpdateWaypointIcon(UUserWidget* TargetWidget, FName ChildIconName, FVector2D NewPos, UTexture2D* MainTexture, UTexture2D* ArrowTexture, float DefaultRadius)
{
	float SquaredXY = NewPos.X * NewPos.X + NewPos.Y * NewPos.Y;
	UImage* WaypointIcon = Cast<UImage>(TargetWidget->GetWidgetFromName(ChildIconName));

	if (!WaypointIcon)
	{
		return;
	}

	if (bIsCircle)
	{
		if (SquaredXY >= (DefaultRadius - 0.1) * (DefaultRadius - 0.1))
		{
			SetWaypointIconOutside(WaypointIcon, ArrowTexture, NewPos);
		}
		else
		{
			SetWaypointIconInside(WaypointIcon, MainTexture);
		}
	}
	else
	{
		if (NewPos.X == DefaultRadius || NewPos.X == -DefaultRadius || NewPos.Y == DefaultRadius || NewPos.Y == -DefaultRadius)
		{
			SetWaypointIconOutside(WaypointIcon, ArrowTexture, NewPos);
		}
		else
		{
			SetWaypointIconInside(WaypointIcon, MainTexture);
		}
	}
}

void UMinimapWidget::UpdateWaypoint(UUserWidget* TargetWidget, AActor* TargetWaypoint, FName ChildIconName, UTexture2D* MainTexture, UTexture2D* ArrowTexture, float DefaultRadius)
{
	FVector WaypointLocation = TargetWaypoint->GetActorLocation();
	FVector PlayerLocation = GetOwningPlayer()->GetPawn()->GetActorLocation();
	FVector WaypointDirection = PlayerLocation - WaypointLocation;
	
	FVector2D ScaledDirection2D = FVector2D(WaypointDirection.X / MapSize.X, WaypointDirection.Y / MapSize.Y);
	ScaledDirection2D = ScaledDirection2D * 10.f * MinimapScale;
	FVector ScaledDirection3D = FVector(ScaledDirection2D.X, ScaledDirection2D.Y, 0.f);

	FRotator PlayerRotation = GetOwningPlayer()->GetControlRotation();
	FRotator Rotator3D = FRotator(-PlayerRotation.Pitch, -(PlayerRotation.Yaw - 90.f), -PlayerRotation.Roll);
	ScaledDirection3D = Rotator3D.RotateVector(ScaledDirection3D);

	FVector2D ClampedDirection2D;
	if (bIsCircle)
	{
		float SquaredXY = ScaledDirection3D.X * ScaledDirection3D.X + ScaledDirection3D.Y * ScaledDirection3D.Y;
		if (SquaredXY > DefaultRadius * DefaultRadius)
		{
			ClampedDirection2D = GetCircleClampedCoords(FVector2D(ScaledDirection3D.X, ScaledDirection3D.Y), DefaultRadius);
		}
		else
		{
			//inside the circle
			ClampedDirection2D = FVector2D(ScaledDirection3D.X, ScaledDirection3D.Y);
		}
	}
	else
	{
		//square
		ClampedDirection2D = FVector2D(FMath::Clamp(ScaledDirection3D.X, -DefaultRadius, DefaultRadius), FMath::Clamp(ScaledDirection3D.Y, -DefaultRadius, DefaultRadius));
	}

	UpdateWaypointIcon(TargetWidget, ChildIconName, ClampedDirection2D, MainTexture, ArrowTexture, DefaultRadius);
	UCanvasPanelSlot* TargetWidgetSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);
	TargetWidgetSlot->SetPosition(ClampedDirection2D);
}

UUserWidget* UMinimapWidget::CreateWaypointWidget(UPanelWidget* MinimapPanel, FName IconName)
{
	UUserWidget* NewWaypointWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), WaypointWidgetClass);
	MinimapPanel->AddChild(NewWaypointWidget);
	if (!NewWaypointWidget)
	{
		return NULL;
	}

	UCanvasPanelSlot* ActorPanelSlot = Cast<UCanvasPanelSlot>(NewWaypointWidget->Slot);
	if (ActorPanelSlot)
	{
		ActorPanelSlot->SetAnchors(FAnchors(0.5, 0.5, 0.5, 0.5));
		ActorPanelSlot->SetAlignment(FVector2D(0.5, 0.5));
		ActorPanelSlot->SetPosition(FVector2D(0, 0));
		ActorPanelSlot->SetSize(FVector2D(1, 1));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Failed to add waypoint!!!")));
	}

	return NewWaypointWidget;
}

void UMinimapWidget::AddWaypoints(TArray<AActor*> TargetWaypoints, UPanelWidget* MinimapPanel, FName IconName)
{
	if (Waypoints.Num() != WaypointsWidgets.Num())
	{
		Waypoints.Empty();
		WaypointsWidgets.Empty();
	}
	else
	{
		for (int ActorIndex = 0; ActorIndex < TargetWaypoints.Num(); ActorIndex++)
		{
			if (TargetWaypoints[ActorIndex])
			{
				UUserWidget* NewActorWidget = CreateWaypointWidget(MinimapPanel, IconName);
				if (NewActorWidget)
				{
					Waypoints.Add(TargetWaypoints[ActorIndex]);
					WaypointsWidgets.Add(NewActorWidget);
				}
			}
		}
	}
}
