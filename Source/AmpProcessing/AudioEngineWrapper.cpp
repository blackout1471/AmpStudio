#include "amppch.h"
#include "AudioEngineWrapper.h"

API AmpProcessing::AudioEngine* CreateAudioEngine()
{
	return new AmpProcessing::AudioEngine();
}

void DestroyAudioEngine(AmpProcessing::AudioEngine* engine)
{
	delete engine;
}

void InitializeAudioEngine(AmpProcessing::AudioEngine* engine)
{
	engine->Init();
}

API float GetInputDbLevel(AmpProcessing::AudioEngine* engine)
{
	return engine->GetInputDbLevel();
}

API float GetOutputDbLevel(AmpProcessing::AudioEngine* engine)
{
	return engine->GetOutputDbLevel();
}

API void SetDesiredInputDbLevel(AmpProcessing::AudioEngine* engine, int dblevel)
{
	return engine->SetDesiredInputDbLevel(dblevel);
}

API void SetDesiredOutputDbLevel(AmpProcessing::AudioEngine* engine, int dblevel)
{
	return engine->SetDesiredOutputDbLevel(dblevel);
}
