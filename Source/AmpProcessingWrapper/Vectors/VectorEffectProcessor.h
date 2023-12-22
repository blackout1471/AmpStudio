#pragma once
#include <Effects/IEffectProcessor.h>
#include <Global.h>

struct VectorEffectProcessor {
	AmpProcessing::Effects::IEffectProcessor** effectProcessors;
	int count;
};

extern "C" {
	API void FreeVectorEffectProcessor(VectorEffectProcessor vector);
}