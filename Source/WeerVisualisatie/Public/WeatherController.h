// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "EWeatherInfoType.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "WeatherInfoData.h"
#include "WeatherController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeatherChangedEvent);

UCLASS()
class WEERVISUALISATIE_API AWeatherController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


// DELEGATES
public:
	UPROPERTY(BlueprintAssignable)
		FOnWeatherChangedEvent OnWeatherChanged;


// VARIABLES
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString APIKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Latitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Longitude;

	/** The amount of seconds between every weather update. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RefreshRate;

private:
	const FString API_CALL = "https://api.openweathermap.org/data/2.5/weather?lat=%.2f&lon=%.2f&units=metric&appid=%s";

	FHttpModule* m_http;
	float m_refreshTimer;

	double m_temperature;	// °C
	double m_humidity;		// %
	double m_windSpeed;		// meter/sec
	double m_windDirection;	// degrees (meteorological)
	double m_clouds;		// %
	double m_rain;			// mm last 1h
	double m_snow;			// mm last 1h


// FUNCTIONS
public:
	void UpdateWeather();

	void OnWeatherResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully);

	UFUNCTION(BlueprintCallable)
		double GetWeatherInfoValue(EWeatherInfoType type);

private:
	double GetValueFromJson(TSharedPtr<FJsonObject> jsonObject, FString objectFieldName, FString numberFieldName);
};
