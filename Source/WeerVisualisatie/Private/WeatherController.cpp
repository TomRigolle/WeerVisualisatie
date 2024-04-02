// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherController.h"

// Sets default values
AWeatherController::AWeatherController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_http = &FHttpModule::Get();

}

// Called when the game starts or when spawned
void AWeatherController::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateWeather();
}

// Called every frame
void AWeatherController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RefreshRate > 0.0)
	{
		m_refreshTimer += DeltaTime;
		if (m_refreshTimer >= RefreshRate)
		{
			m_refreshTimer -= RefreshRate;
			UpdateWeather();
		}
	}
}

void AWeatherController::UpdateWeather()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = m_http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AWeatherController::OnWeatherResponse);
	Request->SetURL(FString::Printf(TEXT("https://api.openweathermap.org/data/2.5/weather?lat=%.2f&lon=%.2f&units=metric&appid=%s"), Latitude, Longitude, *APIKey));
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

void AWeatherController::OnWeatherResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccesfully)
{
	UE_LOG(LogTemp, Display, TEXT("Response %s"), *Response->GetContentAsString());

	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (!FJsonSerializer::Deserialize(reader, jsonObject))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize Json string %s."), *Response->GetContentAsString());
		return;
	}

	double responseCode;
	if (!jsonObject->TryGetNumberField("cod", responseCode) || responseCode != 200)
	{
		UE_LOG(LogTemp, Error, TEXT("API response code is %f."), responseCode);
		return;
	}

	m_temperature = GetValueFromJson(jsonObject, "main", "temp");
	m_humidity = GetValueFromJson(jsonObject, "main", "humidity");
	m_windSpeed = GetValueFromJson(jsonObject, "wind", "speed");
	m_windDirection = GetValueFromJson(jsonObject, "wind", "deg");
	m_clouds = GetValueFromJson(jsonObject, "clouds", "all");
	m_rain = GetValueFromJson(jsonObject, "rain", "1h");
	m_snow = GetValueFromJson(jsonObject, "snow", "1h");

	OnWeatherChanged.Broadcast();
}

double AWeatherController::GetValueFromJson(TSharedPtr<FJsonObject> jsonObject, FString objectFieldName, FString numberFieldName)
{
	const TSharedPtr<FJsonObject>* objectPtr;
	if (!jsonObject->TryGetObjectField(objectFieldName, objectPtr))
	{
		return 0.0;
	}

	return (*objectPtr)->HasField(numberFieldName) ? (*objectPtr)->GetNumberField(numberFieldName) : 0.0;
}

double AWeatherController::GetWeatherInfoValue(EWeatherInfoType type) const
{
	switch (type)
	{
	case EWeatherInfoType::TEMPERATURE:
		return m_temperature;
	case EWeatherInfoType::HUMIDITY:
		return m_humidity;
	case EWeatherInfoType::WIND_SPEED:
		return m_windSpeed;
	case EWeatherInfoType::WIND_DIRECTION:
		return m_windDirection;
	case EWeatherInfoType::CLOUDS:
		return m_clouds;
	case EWeatherInfoType::RAIN:
		return m_rain;
	case EWeatherInfoType::SNOW:
		return m_snow;
	default:	
		UE_LOG(LogTemp, Error, TEXT("GetWeatherInfo: Unable to provide info op type %s as it is not implemented yet."), type);
		return 0.0;
	}
}
