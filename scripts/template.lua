-- template.lua was generated on: 
-- Dec. 11, 2024 - 04:36AM UTC 
workspace "TEMPLATENAME"
	architecture "x86_64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	startproject "TEMPLATEINTERNALNAME"
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	-- Include directories relative to root folder (solution directory)
	IncludeDir = {}
	IncludeDir["spdlog"] = "src/Vendor/spdlog/include"
	IncludeDir["GLAD"] = "src/Vendor/GLAD/include"
	IncludeDir["ImGui"] = "src/Vendor/imgui"
	IncludeDir["glm"] = "src/Vendor/glm"
	IncludeDir["fmt"] = "src/Vendor/fmt/include"
	IncludeDir["stb_image"] = "src/Vendor/stb_image"
	group "Dependencies"
		include "src/Vendor/GLAD"
		include "src/Vendor/imgui"
		include "src/Vendor/fmt"
		include "src/Vendor/spdlog"
	group ""
	project "Ares"
		location "src/Ares"
		kind "StaticLib"
		language "C++"
		cppdialect "C++20"
		staticruntime "on"
		targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
		objdir ("%{wks.location}/bin/" ..outputdir.. "/%{prj.name}/obj")
		pchheader "arespch.h"
		pchsource "src/%{prj.name}/arespch.cpp"
		files
		{
			"src/%{prj.name}/**.h",
			"src/%{prj.name}/**.cpp",
			"src/Vendor/stb_image/**.h",
			"src/Vendor/stb_image/**.cpp"
		}
		includedirs
		{
			"src/%{prj.name}",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.GLAD}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.fmt}",
			"%{IncludeDir.stb_image}"
		}
		links
		{
			"GLAD",
			"ImGui",
			"fmt",
			"spdlog",
			"opengl32.lib"
		}
		defines
		{
			"FMT_UNICODE=0",
			"GLM_ENABLE_EXPERIMENTAL"
		}
		filter "system:windows"
			systemversion "latest"
			defines
			{
				"IMGUI_IMPL_OPENGL_LOADER_GLAD"
			}
		filter "configurations:Debug"
			defines "AR_DEBUG"
			runtime "Debug"
			symbols "on"
		filter "configurations:Release"
			defines "AR_RELEASE"
			runtime "Release"
			optimize "on"
		filter "configurations:Dist"
			defines "AR_DIST"
			runtime "Release"
			optimize "on"
	project "TEMPLATEINTERNALNAME"
		location "src/TEMPLATEINTERNALNAME"
		kind "WindowedApp"
		language "C++"
		cppdialect "C++20"
		staticruntime "on"
		targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
		objdir ("%{wks.location}/bin/" ..outputdir.. "/%{prj.name}/obj")
		files
		{
			"src/%{prj.name}/**.h",
			"src/%{prj.name}/**.cpp"
		}
		includedirs
		{
			"src/%{prj.name}",
			"src/Ares",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.GLAD}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.fmt}"
		}
		links
		{
			"Ares"
		}
		defines
		{
			"FMT_UNICODE=0",
			"GLM_ENABLE_EXPERIMENTAL"
		}
		filter "system:windows"
			systemversion "latest"
			files 
			{
				"src/%{prj.name}/**.rc"
			}
		filter "configurations:Debug"
			defines "AR_DEBUG"
			runtime "Debug"
			symbols "on"
		filter "configurations:Release"
			defines "AR_RELEASE"
			runtime "Release"
			optimize "on"
		filter "configurations:Dist"
			defines "AR_DIST"
			runtime "Release"
			optimize "on"
