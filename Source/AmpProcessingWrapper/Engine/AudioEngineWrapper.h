#pragma once
#include "AudioEngine.h"

#include "Vectors/VectorStringResult.h"
#include "Vectors/VectorEffectProcessor.h"
#include "Dtos/DeviceDetails.h"

extern "C" {
	API AmpProcessing::AudioEngine* CreateAudioEngine();
	API void DestroyAudioEngine(AmpProcessing::AudioEngine* engine);
	API void InitializeAudioEngine(AmpProcessing::AudioEngine* engine);

	API float GetInputDbLevel(AmpProcessing::AudioEngine* engine);
	API float GetOutputDbLevel(AmpProcessing::AudioEngine* engine);

	API void SetDesiredInputDbLevel(AmpProcessing::AudioEngine* engine, int dblevel);
	API void SetDesiredOutputDbLevel(AmpProcessing::AudioEngine* engine, int dblevel);

	API float GetDesiredInputDbLevel(AmpProcessing::AudioEngine* engine);
	API float GetDesiredOutputDbLevel(AmpProcessing::AudioEngine* engine);

	API bool SetSampleRate(AmpProcessing::AudioEngine* engine, uint32_t sampleRate);
	API bool SetBufferSize(AmpProcessing::AudioEngine* engine, uint32_t bufferSize);

	API VectorStringResult GetAvailableDevices(AmpProcessing::AudioEngine* engine);
	API DeviceDetails GetDeviceDetails(AmpProcessing::AudioEngine* engine);

	API VectorEffectProcessor GetAvailableEffects(AmpProcessing::AudioEngine* engine);
}