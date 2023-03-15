processDir = "../Source/AmpProcessing/"

project "AmpProcessing"
	location(srcDir)
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		processDir .. "**.h",
		processDir .. "**.cpp"
	}

	includedirs
	{
		processDir
	}
	
	links
	{
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