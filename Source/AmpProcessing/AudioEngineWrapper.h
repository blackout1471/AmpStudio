#pragma once
#include "AudioEngine.h"

#define API __declspec(dllexport)

extern "C" {
	API AmpProcessing::AudioEngine* CreateAudioEngine();
	API void DestroyAudioEngine(AmpProcessing::AudioEngine* engine);
	API void InitializeAudioEngine(AmpProcessing::AudioEngine* engine);
	API float GetInputDbLevel(AmpProcessing::AudioEngine* engine);
	API float GetOutputDbLevel(AmpProcessing::AudioEngine* engine);
	API void SetDesiredInputDbLevel(AmpProcessing::AudioEngine* engine, int dblevel);
	API void SetDesiredOutputDbLevel(AmpProcessing::AudioEngine* engine, int dblevel);
}