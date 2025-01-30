﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

/**
 * Base class used to get points for triangulation
 */
UCLASS()
class DUNGEONGENERATION_API ARoom : public AActor
{
    GENERATED_BODY()

public:
    ARoom();
};
