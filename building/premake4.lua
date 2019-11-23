solution "6502"
	configurations {
		"release-dynamic", "release-static",
		"debug-dynamic", "debug-static"
	}

	project "6502"
		language "C"
		flags {"ExtraWarnings"}
		files {"../sources/**.c"}
		includedirs {"../API"}
		--buildoptions {"-std=c89 -pedantic -Wall -Wextra -Weverything"}

		configuration "release*"
			targetdir "lib/release"
			flags {"Optimize"}

		configuration "debug*"
			targetdir "lib/debug"
			flags {"Symbols"}

		configuration "*dynamic"
			kind "SharedLib"

		configuration "*static"
			kind "StaticLib"
			defines {"CPU_6502_STATIC"}
