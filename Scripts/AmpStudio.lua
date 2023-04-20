ampstudioDir = "../Source/AmpStudio/"

project "AmpStudio"
	location(ampstudioDir)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		ampstudioDir .. "**.h",
		ampstudioDir .. "**.cpp"
	}

	includedirs
	{
		ampstudioDir,
		processDir,
		"%{IncludeDir.Asio}",
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.FFTConvolver}",
		"%{IncludeDir.ImGui}",
	}
	
	links
	{
		"Asio",
		"FFTConvolver",
		"AmpProcessing",
		"ImGui",
		"d3d9"
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