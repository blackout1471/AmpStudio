wrapperDir = "../Source/AmpProcessingWrapper/"

project "AmpProcessingWrapper"
	location(wrapperDir)
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("../bin/" .. outputdir .. "/%{ProjectNames.AmpApi}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		wrapperDir .. "**.h",
		wrapperDir .. "**.cpp"
	}

	includedirs
	{
		wrapperDir,
		"%{IncludeDir.Process}",
		"%{IncludeDir.Asio}",
		"%{IncludeDir.FFTConvolver}",
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.Vendor}"
	}
	
	links
	{
		"AmpProcessing",
		"Asio",
		"FFTConvolver",
		"Lua"
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