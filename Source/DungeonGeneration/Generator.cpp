// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "Delaunay.h"
#include "MinimumSpanningTree.h"
#include "Room.h"

AGenerator::AGenerator()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGenerator::GenerateRandomRooms()
{
    if (RoomClasses.IsEmpty())
    {
        return;
    }
    Stream.GenerateNewSeed();

    for (int32 i = 0; i < MaximumRooms; ++i)
    {
        FVector Location = GetRandomPointInEllipse(EllipseHeight, EllipseWidth);
        FRotator Rotation(0.0f, 0.0f, 0.0f);
        if (AActor* Room = GetWorld()->SpawnActor<AActor>(RoomClasses[Stream.RandRange(0, RoomClasses.Num() - 1)], Location, Rotation))
        {
            Rooms.Add(Room);
            Room->FindComponentByClass<UPrimitiveComponent>()->SetSimulatePhysics(true);
        }
    }
    for (auto& Room : Rooms)
    {
        Room->SetActorHiddenInGame(true);
    }
    for (int32 i = 0; i < MinimumRooms; ++i)
    {
        Rooms[i]->SetActorHiddenInGame(false);
        SelectedRooms.Add(Rooms[i]);
    }
}


void AGenerator::Triangulate()
{
    TArray<Point*> Points;
    for (auto Actor : SelectedRooms)
    {
        Points.Add(new Point(Actor->GetActorLocation().X, Actor->GetActorLocation().Y));
    }
    Delaunay Delaunay;
    Triangles = Delaunay.Triangulate(Points);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("World delta for current frame equals %i"), Triangles.Num()));
    }
}

void AGenerator::GenerateTree()
{
    MinimumSpanningTree Mst;
    Edges = Mst.BuildTree(Triangles);
}

void AGenerator::GenerateHallways()
{
    for (auto& Edge : Edges)
    {
        TArray<FHitResult> Hits;
        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActors(Hallways);
        GetWorld()->LineTraceMultiByObjectType(Hits, FVector(Edge.p1->x, Edge.p1->y, 0), FVector(Edge.p2->x, Edge.p2->y, 0), ECC_WorldStatic, TraceParams);
        for (auto& Hit : Hits)
        {
            Hit.GetActor()->SetActorHiddenInGame(false);
            MoreRooms.Add(Hit.GetActor());
        }
        CreateHallway(FVector(Edge.p1->x, Edge.p1->y, 0), FVector(Edge.p2->x, Edge.p2->y, 0));
    }
    MoreRooms.RemoveAll([&](AActor* Actor) {
        return SelectedRooms.Contains(Actor);
    });
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
    Super::BeginPlay();
}

void AGenerator::ShowSelectedRooms()
{
    bSelectedRoomsVisible = !bSelectedRoomsVisible;
    for (auto& Room : SelectedRooms)
    {
        Room->SetActorHiddenInGame(!bSelectedRoomsVisible);
    }
}

void AGenerator::ShowAllRooms()
{
    bAllRoomsVisible = !bAllRoomsVisible;
    for (auto& Room : Rooms)
    {
        Room->SetActorHiddenInGame(!bAllRoomsVisible);
    }
    if (bAllRoomsVisible)
    {
        return;
    }
    if (bSelectedRoomsVisible)
    {
        bSelectedRoomsVisible = !bSelectedRoomsVisible;
        ShowSelectedRooms();
    }
    if (bAddedRoomsVisible)
    {
        bAddedRoomsVisible = !bAddedRoomsVisible;
        ShowAddedRooms();
    }
}

void AGenerator::ShowAddedRooms()
{
    bAddedRoomsVisible = !bAddedRoomsVisible;
    for (auto& Room : MoreRooms)
    {
        Room->SetActorHiddenInGame(!bAddedRoomsVisible);
    }
}

void AGenerator::ShowHallways()
{
    bHallwayVisible = !bHallwayVisible;
    for (auto& Room : Hallways)
    {
        if (!Room)
        {
            continue;
        }
        Room->SetActorHiddenInGame(!bHallwayVisible);
    }
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bGraphVisible)
    {
        if (!Triangles.IsEmpty())
        {
            for (auto t : Triangles)
            {
                DrawDebugLine(GetWorld(), FVector(t.p1->x, t.p1->y, 0), FVector(t.p2->x, t.p2->y, 0), FColor::Green);
                DrawDebugLine(GetWorld(), FVector(t.p2->x, t.p2->y, 0), FVector(t.p3->x, t.p3->y, 0), FColor::Green);
                DrawDebugLine(GetWorld(), FVector(t.p3->x, t.p3->y, 0), FVector(t.p1->x, t.p1->y, 0), FColor::Green);
            }
        }
    }
    if (bTreeVisible)
    {
        if (!Edges.IsEmpty())
        {
            for (auto& edge : Edges)
            {
                FVector start(edge.p1->x, edge.p1->y, 2.0f);
                FVector end(edge.p2->x, edge.p2->y, 2.0f);
                DrawDebugLine(GetWorld(), start, end, FColor::Red);
            }
        }
    }
}

void AGenerator::CreateHallway(const FVector& StartPoint, const FVector& EndPoint)
{
    if (!HallwaySegmentClass)
    {
        return;
    }

    FVector Direction = (EndPoint - StartPoint);
    float TotalDistance = FVector::Distance(StartPoint, EndPoint);
    Direction.Normalize();

    FRotator SegmentRotation = Direction.Rotation();

    int32 NumSegments = FMath::CeilToInt(TotalDistance / SegmentLength);

    for (int32 i = 0; i < NumSegments; i++)
    {
        FVector SegmentLocation = StartPoint + (Direction * SegmentLength * i);

        AActor* NewSegment = GetWorld()->SpawnActor<AActor>(HallwaySegmentClass, SegmentLocation, SegmentRotation);

        Hallways.Add(NewSegment);
    }
}

FVector AGenerator::GetRandomPointInEllipse(int32 ellipseHeight = 400, int32 ellipseWidth = 20)
{
    double t = 2 * 3.14 * Stream.RandRange(0.0f, 1.0f);
    double u = Stream.RandRange(0.0f, 1.0f) * Stream.RandRange(0.0f, 1.0f);
    double r = u > 1 ? 2 - u : u;
    return FVector(ellipseWidth * r * FMath::Cos(t) / 2, ellipseHeight * r * FMath::Sin(t) / 2, 0);
}
