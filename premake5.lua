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

	group "Dependencies"
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
			"src/%{prj.name}"
		}

		links
		{
		}

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"AR_PLATFORM_WINDOWS",
				"AR_BUILD_DLL"
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
			"src/Ares"
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