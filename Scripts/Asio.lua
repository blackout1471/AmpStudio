project "Asio"
	
	location "../Vendor/Asio/"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	characterset ("MBCS")

asioSrcDir = "../Vendor/Asio/"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		asioSrcDir .. "**.h",
		asioSrcDir .. "**.cpp"
	}

	includedirs
	{
		asioSrcDir,
		asioSrcDir .. "Include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"