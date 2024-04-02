// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EWeatherInfoType : uint8
{
    TEMPERATURE UMETA(DisplayName = "Temperature"),
    HUMIDITY UMETA(DisplayName = "Humidity"),
    WIND_SPEED UMETA(DisplayName = "Wind Speed"),
    WIND_DIRECTION UMETA(DisplayName = "Wind Direction"),
    CLOUDS UMETA(DisplayName = "Clouds"),
    RAIN UMETA(DisplayName = "Rain"),
    SNOW UMETA(DisplayName = "Snow"),
};