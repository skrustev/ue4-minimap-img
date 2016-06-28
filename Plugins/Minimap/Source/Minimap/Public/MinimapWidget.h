#pragma once

#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta = (Category = "User Controls", DontUseGenericSpawnObject = "True"))
class  UMinimapWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "Minimap Widget")
	UUserWidget* CreateActorWidget(FLinearColor InColor, UPanelWidget* MinimapPanel, FName IconName);

	UFUNCTION(BlueprintCallable, Category = "Minimap Widget")
	void UpdateActor(UUserWidget* TargetActorWidget, APawn* TargetPawn, float DefaultRadius);

	UFUNCTION(BlueprintCallable, Category = "Minimap Widget")
	FVector2D GetCircleClampedCoords(FVector2D InCoords, float InRadius);

	UFUNCTION(BlueprintCallable, Category = "Minimap Widget")
	float GetWaypointIconAngle(FVector2D NewPos);

	UFUNCTION(BlueprintCallable, Category = "Minimap Widget")
	void SetWaypointIconOutside(UImage* WaypointIcon, UTexture2D* Texture, FVector2D NewPos);
	
	UFUNCTION(BlueprintCallable, Category = "Minimap Widget")
	void SetWaypointIconInside(UImage* WaypointIcon, UTexture2D* Texture);

	UFUNCTION(BlueprintCallable, Category = "Minimap Widget")
	void UpdateWaypointIcon(UUserWidget* TargetWidget, FName IconName, FVector2D NewPos, UTexture2D* MainTexture, UTexture2D* ArrowTexture, float Radius);

	UFUNCTION(BlueprintCallable, Category = "Minimap Widget")
	void UpdateWaypoint(UUserWidget* TargetWidget, AActor* TargetWaypoint, FName ChildIconName, UTexture2D* MainTexture, UTexture2D* ArrowTexture, float DefaultRadius);
	
	UFUNCTION(BlueprintCallable, Category = "Minimap Widget")
	UUserWidget* CreateWaypointWidget(UPanelWidget* MinimapPanel, FName IconName);

	UFUNCTION(BlueprintCallable, Category = "Minimap Widget")
	void AddWaypoints(TArray<AActor*> TargetWaypoints, UPanelWidget* MinimapPanel, FName IconName);

	TSubclassOf<class UUserWidget> ActorWidgetClass;

	TSubclassOf<class UUserWidget> WaypointWidgetClass;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap Settings")
	FString MinimapWidgetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap Settings")
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap Settings")
	UTexture2D* CircleMask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap Settings")
	float MinimapScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap Settings")
	bool bIsCircle;

	UPROPERTY(BlueprintReadWrite, Category = "Minimap Widget")
	TArray<AActor*> Actors;

	UPROPERTY(BlueprintReadWrite, Category = "Minimap Widget")
	TArray<UUserWidget*> ActorsWidgets;

	UPROPERTY(BlueprintReadWrite, Category = "Minimap Widget")
	TArray<AActor*> Waypoints;

	UPROPERTY(BlueprintReadWrite, Category = "Minimap Widget")
	TArray<UUserWidget*> WaypointsWidgets;

	UPROPERTY(BlueprintReadWrite, Category = "Minimap Settings")
	FVector MapStartLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Minimap Settings")
	FVector MapSize;

	UFUNCTION(BlueprintCallable, Category = "Minimap Widget")
	void AddActors(TArray<AActor*> TargetActors, FLinearColor InColor, UPanelWidget* MinimapPanel, FName IconName);
};
