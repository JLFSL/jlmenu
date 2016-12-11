// Game Information
#define INFO_GAME_NAME			"Grand Theft Auto 5"
#define INFO_GAME_EXECUTABLE	"GTAVLauncher.exe"
#define INFO_GAME_STEAMAPPID	"271590"

	// Game Path
	#define INFO_GAME_LOC_REG		"SOFTWARE\\WOW6432Node\\Rockstar Games\\Grand Theft Auto V"
	#define INFO_GAME_LOC_REG2		"InstallFolder"

// Modification Information
#define INFO_NAME			"jLmenu Private"
#define INFO_NAME_SHORT		"jlMenu"
#define INFO_VERSION		"0.1a"

	// Client Path
	#define INFO_LOC_REG "Software\\" INFO_NAME_SHORT
	#define INFO_LOC_REG2 INFO_NAME_SHORT

// Modification Files
#define INFO_CLIENT_CORE	"Base" LIBRARY_EXTENSION

#ifdef WIN32
#define LIBRARY_EXTENSION ".dll"
#else
#define LIBRARY_EXTENSION ".so"
#endif

#define SAFE_DELETE(memory) if(memory) { delete memory; memory = NULL; }

#ifdef NDEBUG
#define INFO_BUILD "Release"
#else
#define INFO_BUILD "Debug"
#endif