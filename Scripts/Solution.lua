workspace "AmpStudio"
	location "..\\"
	architecture "x86_64"
	startproject "Sandbox"

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

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Asio"] = "../Vendor/Asio/Include"
IncludeDir["FFTConvolver"] = "../Vendor/FFTConvolver"
IncludeDir["ImGui"] = "../Vendor/imgui/"

IncludeDir["GTest"] = "../Vendor/googletest/googletest/"
IncludeDir["SPDLOG"] = "../Vendor/spdlog/include"
IncludeDir["Process"] = "../Source/AmpProcessing/"

include "AmpProcessing.lua"
include "AmpStudio.lua"

group "Dependencies"
	include "Asio.lua"
	include "FFTConvolver.lua"
	include "ImGui.lua"
group ""

group "Demos"
	include "Sandbox.lua"
group ""

group "Test"
	include "UnitTest.lua"
group ""