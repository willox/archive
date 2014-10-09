solution "gmsv_refresh"
	language "C++"
	location ( "project-" .. os.get() )
	targetdir ( "build/" .. os.get() .. "/" )

	flags { "StaticRuntime" }

	includedirs {
		"include"
	}

	libdirs {
		"lib"
	}

	links {
		"Psapi"
	}

	configurations { 
		"Release"
	}
	
	configuration "Release"
		defines { "GMMODULE" }
		optimize "On"
	
	project "gmsv_refresh"
		kind "SharedLib"
		
		files {
			"src/**.cpp",
			"src/**.h"
		}

		if os.is "windows" then
			targetsuffix "_win32"
		elseif os.is "linux" then
			targetsuffix "_linux"
		elseif os.is "macosx" then
			targetsuffix "_osx"
		end