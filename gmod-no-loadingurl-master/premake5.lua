solution "shader_loading_url"
	language "C++"
	location "project"
	targetdir ( "build/" .. os.get() .. "/" )

	flags { "StaticRuntime" }

	includedirs {

	}

	libdirs {

	}

	links {

	}
	
	configurations { 
		"Release"
	}

	configuration "Release"
		defines { "GMMODULE" }
		optimize "On"

	project "shader_loading_url"
		kind "SharedLib"
		
		files {
			"src/**.cpp",
			"src/**.h"
		}

		targetname "game_shader_generic_loading_url"