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
	IncludeDir["fmt"] = "src/Vendor/fmt/include"
	IncludeDir["stb_image"] = "src/Vendor/stb_image"
	IncludeDir["EASTL"] = "src/Vendor/eastl/include"
	IncludeDir["EABase"] = "src/Vendor/eabase/include/Common"

	group "Dependencies"
		include "src/Vendor/GLAD"
		include "src/Vendor/imgui"
		include "src/Vendor/fmt"
		include "src/Vendor/spdlog"
		include "src/Vendor/eabase"
		include "src/Vendor/eastl"
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
			"%{IncludeDir.stb_image}",
			"%{IncludeDir.EABase}",
			"%{IncludeDir.EASTL}"
		}

		links
		{
			"GLAD",
			"ImGui",
			"fmt",
			"spdlog",
			"EASTL",
			"opengl32.lib"
		}

		defines
		{
			"FMT_UNICODE=0"
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
			optimize "speed"

	project "Sandbox"
		location "src/Sandbox"
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
			"%{IncludeDir.fmt}",
			"%{IncludeDir.EABase}",
			"%{IncludeDir.EASTL}"
		}

		links
		{
			"Ares"
		}

		defines
		{
			"FMT_UNICODE=0"
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
			optimize "speed"