imguiDir = "../Vendor/imgui/"

project "ImGui"
	
	location (imguiDir)
	kind "StaticLib"
	language "C++"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
        imguiDir .. "imconfig.h",
		imguiDir .. "imgui.h",
		imguiDir .. "imgui_internal.h",
		imguiDir .. "imstb_rectpack.h",
		imguiDir .. "imstb_textedit.h",
		imguiDir .. "imstb_truetype.h",
		imguiDir .. "backends/imgui_impl_dx9.h",
		imguiDir .. "backends/imgui_impl_win32.h",

		imguiDir .. "imgui.cpp",
		imguiDir .. "imgui_draw.cpp",
		imguiDir .. "imgui_widgets.cpp",
		imguiDir .. "imgui_tables.cpp",
		imguiDir .. "imgui_demo.cpp",
		imguiDir .. "backends/imgui_impl_dx9.cpp",
		imguiDir .. "backends/imgui_impl_win32.cpp"
	}

	includedirs
	{
		imguiDir
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