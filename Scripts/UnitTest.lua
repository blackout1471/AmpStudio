UnitDir = "../Source/UnitTest/"

project "UnitTest"
	location(UnitDir)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		UnitDir .. "**.h",
		UnitDir .. "**.cpp",
		"%{IncludeDir.GTest}" .. "**.h",
		"%{IncludeDir.GTest}" .. "src/gtest-all.cc"
	}

	includedirs
	{
		UnitDir,
		"../Source/AmpProcessing/",
		"%{IncludeDir.GTest}",
		"%{IncludeDir.GTest}" .. "include/"
	}
	
	links
	{
		"AmpProcessing"
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