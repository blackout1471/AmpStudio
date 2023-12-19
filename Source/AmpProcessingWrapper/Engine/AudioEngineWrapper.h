#pragma once
#include "AudioEngine.h"

#include "Vectors/VectorStringResult.h"
#include <Dtos/DeviceDetails.h>

extern "C" {
	API AmpProcessing::AudioEngine* CreateAudioEngine();
	API void DestroyAudioEngine(AmpProcessing::AudioEngine* engine);
	API void InitializeAudioEngine(AmpProcessing::AudioEngine* engine);

	API float GetInputDbLevel(AmpProcessing::AudioEngine* engine);
	API float GetOutputDbLevel(AmpProcessing::AudioEngine* engine);
	API void SetDesiredInputDbLevel(AmpProcessing::AudioEngine* engine, int dblevel);
	API void SetDesiredOutputDbLevel(AmpProcessing::AudioEngine* engine, int dblevel);

	API VectorStringResult GetAvailableDevices(AmpProcessing::AudioEngine* engine);
	
	API DeviceDetails GetDeviceDetails(AmpProcessing::AudioEngine* engine);
}