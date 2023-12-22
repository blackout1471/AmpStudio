#include "ManagedAudioEngine.h"

ManagedAudioEngine::ManagedAudioEngine()
{
	m_NativeAudioEngine = new AmpProcessing::AudioEngine();
}

ManagedAudioEngine::~ManagedAudioEngine()
{
	delete m_NativeAudioEngine;
}

ManagedAudioEngine::!ManagedAudioEngine()
{
	delete m_NativeAudioEngine;
}

void ManagedAudioEngine::Init()
{
	m_NativeAudioEngine->Init();
}

void ManagedAudioEngine::SetDesiredInputDbLevel(int desiredLevel)
{
	m_NativeAudioEngine->SetDesiredInputDbLevel(desiredLevel);
}

void ManagedAudioEngine::SetDesiredOutputDbLevel(int desiredLevel)
{
	m_NativeAudioEngine->SetDesiredOutputDbLevel(desiredLevel);
}

float ManagedAudioEngine::InputDbLevel::get()
{
	return m_NativeAudioEngine->GetInputDbLevel();
}

float ManagedAudioEngine::OutputDbLevel::get()
{
	return m_NativeAudioEngine->GetOutputDbLevel();
}
