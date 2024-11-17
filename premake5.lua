workspace "Ares"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Sandbox"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	-- Include directories relative to root folder (solution directory)
	IncludeDir = {}
	IncludeDir["spdlog"] = "src/Vendor/spdlog/include"
	IncludeDir["GLAD"] = "src/Vendor/GLAD/include"
	IncludeDir["ImGui"] = "src/Vendor/imgui"
	IncludeDir["glm"] = "src/Vendor/glm"

	group "Dependencies"
		include "src/Vendor/GLAD"
		include "src/Vendor/imgui"
		include "src/Vendor/spdlog"
	group ""

	project "Ares"
		location "src/Ares"
		kind "StaticLib"
		language "C++"
		cppdialect "C++20"
		staticruntime "on"

		targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
		objdir ("%{wks.location}/src/bin/%{prj.name}/")

		pchheader "arespch.h"
		pchsource "src/%{prj.name}/arespch.cpp"

		files
		{
			"src/%{prj.name}/**.h",
			"src/%{prj.name}/**.cpp"
		}

		includedirs
		{
			"src/%{prj.name}",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.GLAD}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.glm}"
		}

		links
		{
			"GLAD",
			"ImGui",
			"spdlog",
			"opengl32.lib"
		}

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"AR_PLATFORM_WINDOWS",
				"AR_BUILD_DLL",
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

	project "Sandbox"
		location "src/Sandbox"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++20"
		staticruntime "on"

		targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
		objdir ("%{wks.location}/src/bin/%{prj.name}/")

		files
		{
			"src/%{prj.name}/**.h",
			"src/%{prj.name}/**.cpp"
		}

		includedirs
		{
			"src/Ares",
			"src/Vendor",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.glm}"
		}

		links
		{
			"Ares"
		}

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"AR_PLATFORM_WINDOWS"
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