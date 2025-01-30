// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.h"
#include "DataStruct.h"
#include "Generator.generated.h"


UCLASS()
class DUNGEONGENERATION_API AGenerator : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGenerator();
    /**
     * Generate Random a random room inside an ellipsis 
     */
    UFUNCTION(BlueprintCallable)
    void GenerateRandomRooms();
    UFUNCTION(BlueprintCallable)
    void Triangulate();
    UFUNCTION(BlueprintCallable)
    void GenerateTree();
    UFUNCTION(BlueprintCallable)
    void GenerateHallways();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    /**
     * Classes used for room generation
     */
    UPROPERTY(EditAnywhere, Category = "GeneratorConfig")
    TArray<TSubclassOf<ARoom>> RoomClasses;
    /**
     * Height of ellipse used to spawn rooms
     */
    UPROPERTY(EditAnywhere, Category = "GeneratorConfig")
    int32 EllipseHeight = 1000;
    /**
     * Width of ellipse used to spawn rooms
     */
    UPROPERTY(EditAnywhere, Category = "GeneratorConfig")
    int32 EllipseWidth = 1000;
    /**
     * Number of rooms used for triangulation
     */
    UPROPERTY(EditAnywhere, Category = "GeneratorConfig")
    int32 MinimumRooms = 10;
    /**
     * Number of rooms spawned
     */
    UPROPERTY(EditAnywhere, Category = "GeneratorConfig")
    int32 MaximumRooms = 100;


    UPROPERTY(EditAnywhere, Category = "Hallway")
    TSubclassOf<AActor> HallwaySegmentClass;
    float SegmentLength = 20.0f;
    virtual void Tick(float DeltaTime) override;


    #pragma region Debug
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAllRoomsVisible = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSelectedRoomsVisible = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHallwayVisible = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAddedRoomsVisible = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bGraphVisible = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bTreeVisible = true;
    UFUNCTION(BlueprintCallable)
    void ShowSelectedRooms();
    UFUNCTION(BlueprintCallable)
    void ShowAllRooms();
    UFUNCTION(BlueprintCallable)
    void ShowAddedRooms();
    UFUNCTION(BlueprintCallable)
    void ShowHallways();
    #pragma endregion

private:
    /**
     * Rooms used for triangulation
     */
    TArray<AActor*> SelectedRooms;
    /**
     * All rooms generated
     */
    TArray<AActor*> Rooms;
    /**
     * All hallways generaed
     */
    TArray<AActor*> Hallways;

    /**
     * Rooms which are added to dungeon if they are on the path between 2 selected rooms
     */
    TArray<AActor*> MoreRooms;
    FVector GetRandomPointInEllipse(int32 ellipseHeight, int32 ellipseWidth);
    TArray<Edge> Edges;
    TArray<Triangle> Triangles;
    void CreateHallway(const FVector& StartPoint, const FVector& EndPoint);
    FRandomStream Stream;
};
