project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/obj")

	files
	{
		"include/glad/gl.h",
		"include/glad/wgl.h.h",
		"include/KHR/khrplatform.h",
		"src/gl.c",
		"src/wgl.c"
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "speed"

--[[
	Created using glad.dav1d.de - glad 2.0.8 on Mon Mar 31 20:12:01 2025
	Generator: C/C++
	Specification: gl
	Extensions: 620 (ALL)
	
	APIs:
	 - gl:core=4.6

	Options:
	 - ALIAS = True
	 - DEBUG = True
	 - HEADER_ONLY = False
	 - LOADER = True
	 - MX = False
	 - ON_DEMAND = False

	Options:
	- loader
--]]