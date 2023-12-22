ampApiDir = "../Source/AmpApi/"

require "vstudio"

local function platformsElement(cfg)
  _p(2,'<Platforms>x64</Platforms>')
  _p(2,'<AppendTargetFrameworkToOutputPath>false</AppendTargetFrameworkToOutputPath>')
end

local function webProjectElement(cfg)
  premake.w('<Project Sdk="Microsoft.NET.Sdk.Web">')
end

premake.override(premake.vstudio.cs2005.elements, "projectProperties", function (oldfn, cfg)
  return table.join(oldfn(cfg), {
    platformsElement,
  })
end)

premake.override(premake.vstudio.cs2005.elements, "project", function (base, prj)
	local calls = base(prj)
	table.remove(calls, 1)
	table.insert(calls, 1, webProjectElement)
	return calls
end)

project (ProjectNames.AmpApi)
   location(ampApiDir)
   kind "ConsoleApp"
   language "C#"
   dotnetframework "net8.0"

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   files { ampApiDir .. "**.cs" }
   links { "AmpProcessingWrapper" }

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

   filter {}
        nuget 
		{ 
			"Swashbuckle.AspNetCore.Swagger:6.5.0",
			"Swashbuckle.AspNetCore.SwaggerGen:6.5.0",
			"Swashbuckle.AspNetCore.SwaggerUI:6.5.0",
			"Asp.Versioning.Mvc:8.0.0"
		}