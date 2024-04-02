// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EWeatherInfoType.h"
#include "WeatherInfoData.generated.h"

USTRUCT(BlueprintType)
struct WEERVISUALISATIE_API FWeatherInfoData
{
	GENERATED_USTRUCT_BODY()

public:
	FWeatherInfoData();
	~FWeatherInfoData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeatherInfoType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Units;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		double Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture* Icon;
};
