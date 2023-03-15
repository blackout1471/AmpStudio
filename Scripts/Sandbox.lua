sandboxDir = "../Source/Sandbox/"

project "Sandbox"
	location(srcDir)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		sandboxDir .. "**.h",
		sandboxDir .. "**.cpp"
	}

	includedirs
	{
		sandboxDir,
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