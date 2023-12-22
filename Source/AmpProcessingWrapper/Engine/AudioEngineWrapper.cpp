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

API float GetDesiredInputDbLevel(AmpProcessing::AudioEngine* engine)
{
	return engine->GetDesiredInputDbLevel();
}

API float GetDesiredOutputDbLevel(AmpProcessing::AudioEngine* engine)
{
	return engine->GetDesiredOutputDbLevel();
}

API VectorStringResult GetAvailableDevices(AmpProcessing::AudioEngine* engine)
{
	return ConvertToVectorString(engine->GetAvailableDevices());
}

API DeviceDetails GetDeviceDetails(AmpProcessing::AudioEngine* engine)
{
	auto& details = engine->GetDeviceDetails();

	return {
		ConvertToVectorString(details.name),
		details.inputChannels,
		details.outputChannels,
		details.minBufferSize,
		details.maxBufferSize,
		details.prefferedBufferSize,
		details.granularityBuffer,
		details.sampleRate
	};
}
