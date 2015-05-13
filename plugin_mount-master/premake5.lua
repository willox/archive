assert(os.getenv("SOURCE_SDK"), "SOURCE_SDK environmental variable not set")

local SOURCE_SDK = os.getenv("SOURCE_SDK") .. "/mp/src"

solution "plugin_mount"
	language "C++"
	location "project"
	targetdir "build"

	flags { "StaticRuntime" }

	includedirs {
		SOURCE_SDK .. "/common",
		SOURCE_SDK .. "/public",
		SOURCE_SDK .. "/public/tier0",
		SOURCE_SDK .. "/public/tier1",
		SOURCE_SDK .. "/public/tier2"
	}

	libdirs {
		SOURCE_SDK .. "/lib/public"
	}

	links {
		"tier0",
		"tier1",
		"tier2"
	}
	
	configurations {
		"Release"
	}

	configuration "Release"
		optimize "On"
		linkoptions { "/NODEFAULTLIB:LIBCMT" }
	
	project "plugin_mount"
		kind "SharedLib"
		
		files {
			"src/**.cpp",
			"src/**.h"
		}