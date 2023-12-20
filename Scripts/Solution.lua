workspace "AmpStudio"
	location "..\\"
	architecture "x86_64"
	startproject "AmpApi"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

ProjectNames = {
	AmpProcessing = "AmpProcessing",
	AmpProcessingWrapper = "AmpProcessingWrapper",
	AmpApi = "AmpApi"
}

IncludeDir = {
	Asio = "../Vendor/Asio/Include",
	FFTConvolver = "../Vendor/FFTConvolver",
	GTest = "../Vendor/googletest/googletest/",
	SPDLOG = "../Vendor/spdlog/include",
	Process = "../Source/AmpProcessing/",
	Vendor = "../Vendor/"
}

include "AmpProcessing.lua"
include "AmpApi.lua"
include "AmpProcessingWrapper.lua"

group "Dependencies"
	include "Asio.lua"
	include "FFTConvolver.lua"
	include "Lua.lua"
group ""

group "Demos"
	include "Sandbox.lua"
group ""

group "Test"
	include "UnitTest.lua"
group ""