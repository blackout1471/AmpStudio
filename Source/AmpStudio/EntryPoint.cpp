#include "aspch.h"
#include <iostream>
#include <asiosys.h>
#include <asio.h>
#include <asiodrivers.h>

static AsioDrivers s_AsioDriver;

const std::vector<std::string> getAduioDeviceNames() {
	int maxSize = 32;
	auto deviceNames = std::vector<std::string>();

	char** names = new char* [maxSize];
	for (int i = 0; i < 32; i++)
	{
		names[i] = new char[maxSize];
	}

	s_AsioDriver.getDriverNames(names, maxSize);

	for (int i = 0; i < maxSize; i++)
	{
		if (names[i][0] != -51)
			deviceNames.push_back(names[i]);
		delete[] names[i];
	}

	delete[] names;

	return deviceNames;
}

int main() {

	// Get devices
	const auto& devices = getAduioDeviceNames();
	for (size_t i = 0; i < devices.size(); i++)
	{
		std::cout << (devices[i]) << std::endl;
	}

	// Load first device
	char* name = strdup(devices.front().c_str());
	bool success = s_AsioDriver.loadDriver(name);
	delete name;

	// initialise device details
	ASIODriverInfo inf;
	int code = ASIOInit(&inf);
	std::cout << "[ASIO] Device loaded " << inf.name << std::endl;
	std::cout << "[ASIO] Driver version " << inf.driverVersion << std::endl;
	std::cout << "[ASIO] Asio version " << inf.asioVersion << std::endl;
	std::cout << "[ASIO] Asio Error " << inf.errorMessage << std::endl;

	// Get device settings
	long numInputChannel, numOutputChannel;
	code = ASIOGetChannels(&numInputChannel, &numOutputChannel);

	long minSizeBuffer, maxSizeBuffer, prefferedBufferSize, granularityBuffer;
	code = ASIOGetBufferSize(&minSizeBuffer, &maxSizeBuffer, &prefferedBufferSize, &granularityBuffer);

	ASIOSampleRate sampleRate;
	ASIOGetSampleRate(&sampleRate);
	//ASIOSetSampleRate(sampleRate);

	// Start asio
	if (ASIOStart() != 0)
		std::cout << "Could not start asio" << std::endl;

	return 0;
}