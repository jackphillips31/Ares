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
		location "Ares"
		kind "StaticLib"
		language "C++"
		cppdialect "C++20"
		staticruntime "on"

		targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
		objdir ("%{wks.location}/%{prj.name}/bin/")

		pchheader "arespch.h"
		pchsource "%{prj.name}/src/arespch.cpp"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"%{prj.name}/src"
		}

		links
		{
		}

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"ARES_PLATFORM_WINDOWS",
				"ARES_BUILD_DLL"
			}

		filter "configurations:Debug"
			defines "ARES_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "ARES_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "ARES_DIST"
			runtime "Release"
			optimize "on"

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++20"
		staticruntime "on"

		targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
		objdir ("%{wks.location}/%{prj.name}/bin/")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"Ares/src"
		}

		links
		{
			"Ares"
		}

		filter "system:windows"
			systemversion "latest"

			defines
			{
				"ARES_PLATFORM_WINDOWS"
			}

		filter "configurations:Debug"
			defines "ARES_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "ARES_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "ARES_DIST"
			runtime "Release"
			optimize "on"