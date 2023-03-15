workspace "AmpStudio"
	location "..\\"
	architecture "x86_64"
	startproject "AmpStudio"

	characterset ("MBCS")

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
srcDir = "../Source/AmpStudio/"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Asio"] = "../Vendor/Asio/Include"

group "Dependencies"
	include "Asio.lua"
group ""

group "Demos"
	include "Sandbox.lua"
group ""

project "AmpStudio"
	location(srcDir)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "aspch.h"
	pchsource(srcDir .. "aspch.cpp")

	files
	{
		srcDir .. "**.h",
		srcDir .. "**.cpp"
	}

	includedirs
	{
		srcDir,
		"%{IncludeDir.Asio}"
	}
	
	links
	{
		"Asio"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "on"