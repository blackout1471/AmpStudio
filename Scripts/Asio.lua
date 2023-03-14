project "Asio"
	
	location "../Vendor/Asio/"
	kind "StaticLib"
	language "C++"

	characterset ("MBCS")

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"../vendor/Asio/include/**.h",
		"../vendor/Asio/**.cpp"
	}

	includedirs
	{
		"../vendor/Asio",
		"../vendor/Asio/include"
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