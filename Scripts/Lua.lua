project "Lua"
	
	location "../vendor/lua/"
	kind "StaticLib"
	language "C"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"../vendor/lua/*.h",
		"../vendor/lua/*.c"
	}
	
	excludes
	{
		"../vendor/lua/lua.c",
		"../vendor/lua/luac.c",
		"../vendor/lua/onelua.c"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"