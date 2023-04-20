processDir = "../Source/AmpProcessing/"

project "AmpProcessing"
	location(processDir)
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "amppch.h"
	pchsource(processDir .. "amppch.cpp")

	files
	{
		processDir .. "**.h",
		processDir .. "**.cpp"
	}

	includedirs
	{
		processDir,
		"%{IncludeDir.Asio}",
		"%{IncludeDir.FFTConvolver}",
		"%{IncludeDir.SPDLOG}",
	}
	
	links
	{
		"Asio",
		"FFTConvolver"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"
		
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