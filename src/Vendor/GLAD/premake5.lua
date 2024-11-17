project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/" .. outputdir .. "/obj")

	files
	{
		"include/glad/gl.h",
		"include/glad/wgl.h",
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

--[[
	Created using glen.glad.sh
	Generator: C/C++
	gl: Version 4.5
	wgl: Version 1.0
	Compatibility: CORE

	Extensions:
	- WGL_ARB_create_context
	- WGL_EXT_swap_control

	Options:
	- loader
--]]