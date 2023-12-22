fftSrcDir = "../Vendor/FFTConvolver/"

project "FFTConvolver"
	
	location (fftSrcDir)
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"


	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		fftSrcDir .. "*.h",
		fftSrcDir .. "*.cpp"
	}

	includedirs
	{
		fftSrcDir
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "off"

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