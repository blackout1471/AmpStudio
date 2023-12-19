#pragma once
#include <Vectors/VectorStringResult.h>

struct DeviceDetails {
	VectorStringResult name;

	long inputChannels;
	long outputChannels;

	long minBufferSize;
	long maxBufferSize;
	long prefferedBufferSize;
	long granularityBuffer;

	float sampleRate;
};