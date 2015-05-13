assert(os.getenv("SOURCE_SDK"), "SOURCE_SDK environmental variable not set")

local SOURCE_SDK = os.getenv("SOURCE_SDK") .. "/mp/src/"


solution "gmod_no_screencap"
	language "C++"
	location "project"
	targetdir "bin"


	includedirs {
		"src",

        SOURCE_SDK .. "/common",
        SOURCE_SDK .. "/public",
        SOURCE_SDK .. "/public/tier0",
        SOURCE_SDK .. "/public/tier1"
	}

	libdirs {
        SOURCE_SDK .. "/lib/public"
	}

	links {
        "tier0",
        "tier1"
	}


	configurations { 
		"Debug",
		"Release"
	}

	configuration "Debug"
		flags { "symbols" }

	configuration "Release"
		optimize "On"

	project "gmod_no_screencap"
		kind "SharedLib"
		
		files {
			"src/**.cpp",
			"src/**.h"
		}

		targetname "no_screencap"