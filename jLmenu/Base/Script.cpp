#include "stdafx.h"

// DX11Renderer.cpp
DWORD __stdcall InitializeHook(LPVOID);
void* detourBuffer[3];

DWORD __stdcall MouseHook(LPVOID)
{
	MouseHook::Instance().InstallHook();
	MouseHook::Instance().Messages();
	return NULL;
}

DWORD __stdcall KeyboardHook(LPVOID)
{
	KeyboardHook::Instance().InstallHook();
	//KeyboardHook::Instance().Messages();
	return NULL;
}

DWORD __stdcall ForegroundCheck(LPVOID) {
	MouseHook::Instance().ForegroundCheck();
	return NULL;
}


bool Script::isInit()
{
	memset(detourBuffer, 0, (sizeof(detourBuffer) * sizeof(void*)));
	CreateThread(NULL, 0, MouseHook, NULL, 0, NULL);
	CreateThread(NULL, 0, KeyboardHook, NULL, 0, NULL);
	CreateThread(NULL, 0, InitializeHook, NULL, 0, NULL);
	//CreateThread(NULL, 0, ForegroundCheck, NULL, 0, NULL);
	return true;
}

#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define MAX_PLAYERS 32
struct Players {
	int id;
	Ped ped;

	char *name;
	int health;
	int maxhealth;

	Vector3 coordinates;
} players[1000];


//static bool showjlmenu = true;

static bool jl_NeverWanted = false;
static bool jl_GodMode = false;
static bool jl_GodMode_t = false;
static bool jl_Visible = true;
static bool jl_DisableRagdoll = false;
static bool jl_Suicide = false;
static bool jl_SuperJump = false;
static bool jl_FastRun = false;
static float jl_FastRun_f = 1.0f;
static bool jl_FastSwim = false;
static float jl_FastSwim_f = 1.0f;
static bool jl_FireAmmo = false;
static bool jl_ExplosiveAmmo = false;
static bool jl_ExplosiveMelee = false;
static bool jl_StatRP = false;
static int jl_StatRP_d = 50;
static bool jl_StatCash = false;
static int jl_StatCash_d = 200;
static DWORD jl_StatCash_t;
static bool jl_StatOtherCash = false;
static int jl_StatOtherCashTo = 0;
static bool jl_Yankton = false;

static bool jl_CreateVehicle = false;
static int jl_CreateVehicleId = -1;

static bool jl_SpeedUpVehicle = false;
static bool jl_RepairVehicle = false;
static bool jl_ReduceGrip = false;
static bool jl_ReduceGrip_Set = false;
static bool jl_VVisible = true;

static bool jl_TeleportToPlayer = false;
static int jl_TeleportTo = 0;
static bool jl_TeleportPlayerHere = false;
static int jl_TeleportHere = 0;

static bool settime = false;
const char* VehiclesComboList[] = { "pfister811" , "adder" , "banshee2" , "bullet" , "cheetah" , "entityxf" , "sheava" , "fmj" , "infernus" , "osiris" , "le7b" , "reaper" , "sultanrs" , "t20" , "turismor" , "tyrus" , "vacca" , "voltic" , "prototipo" , "zentorno" , "cogcabrio" , "exemplar" , "f620" , "faction" , "faction2" , "felon" , "felon2" , "jackal" , "sentinel" , "sentinel2" , "windsor" , "windsor2" , "zion" , "zion2" , "alpha" , "banshee" , "bestiagts" , "blista2" , "blista3" , "buffalo" , "buffalo2" , "buffalo3" , "carbonizzare" , "comet2" , "coquette" , "elegy2" , "feltzer2" , "furoregt" , "fusilade" , "futo" , "jester" , "jester2" , "khamelion" , "kuruma" , "kuruma2" , "lynx" , "mamba" , "massacro" , "massacro2" , "ninef" , "ninef2" , "omnis" , "penumbra" , "rapidgt" , "rapidgt2" , "schwarzer" , "seven70" , "sultan" , "surano" , "tropos" , "verlierer2" , "blade" , "buccaneer" , "buccaneer2" , "chino" , "chino2" , "dominator" , "dominator2" , "tampa2" , "dukes" , "dukes2" , "faction3" , "btype2" , "gauntlet" , "gauntlet2" , "hotknife" , "minivan2" , "nightshade" , "phoenix" , "picador" , "ruiner" , "sabregt" , "sabregt2" , "slamvan" , "slamvan2" , "slamvan3" , "stalion" , "stalion2" , "tampa" , "tornado5" , "vigero" , "virgo" , "virgo2" , "virgo3" , "voodoo" , "voodoo2" , "casco" , "coquette2" , "coquette3" , "feltzer3" , "jb700" , "lurcher" , "manana" , "monroe" , "peyote" , "pigalle" , "btype" , "btype3" , "stinger" , "stingergt" , "tornado" , "tornado2" , "tornado3" , "tornado4" , "ztype" , "benson" , "biff" , "boxville" , "boxville2" , "boxville3" , "boxville4" , "cutter" , "docktug" , "dump" , "rallytruck" , "flatbed" , "handler" , "hauler" , "mixer" , "mixer2" , "mule" , "mule2" , "mule3" , "packer" , "phantom" , "pounder" , "rubble" , "scrap" , "stockade" , "stockade3" , "tiptruck" , "tiptruck2" , "towtruck" , "towtruck2" , "asea" , "asea2" , "asterope" , "cog55" , "cog552" , "cognoscenti" , "cognoscenti2" , "emperor" , "emperor2" , "emperor3" , "fugitive" , "glendale" , "ingot" , "intruder" , "limo2" , "oracle" , "oracle2" , "premier" , "primo" , "primo2" , "regina" , "romero" , "schafter2" , "schafter3" , "schafter4" , "schafter5" , "schafter6" , "stanier" , "stratum" , "stretch" , "superd" , "surge" , "tailgater" , "taxi" , "warrener" , "washington" , "bfinjection" , "bifta" , "blazer" , "blazer2" , "blazer3" , "bodhi2" , "brawler" , "trophytruck2" , "dloader" , "dubsta3" , "dune" , "dune2" , "guardian" , "insurgent" , "insurgent2" , "kalahari" , "marshall" , "mesa3" , "monster" , "rancherxl" , "rancherxl2" , "rebel" , "rebel2" , "sandking" , "sandking2" , "technical" , "trophytruck" , "baller" , "baller2" , "baller3" , "baller4" , "baller5" , "baller6" , "bjxl" , "cavalcade" , "cavalcade2" , "dubsta" , "dubsta2" , "fq2" , "granger" , "gresley" , "habanero" , "huntley" , "landstalker" , "mesa" , "mesa2" , "patriot" , "radi" , "rocoto" , "seminole" , "serrano" , "xls" , "xls2" , "blista" , "brioso" , "dilettante" , "dilettante2" , "issi2" , "panto" , "prairie" , "rhapsody" , "bison" , "bison2" , "bison3" , "bobcatxl" , "contender" , "ratloader" , "ratloader2" , "sadler" , "sadler2" , "burrito" , "burrito2" , "burrito3" , "burrito4" , "burrito5" , "camper" , "gburrito" , "gburrito2" , "journey" , "minivan" , "moonbeam" , "moonbeam2" , "paradise" , "pony" , "pony2" , "rumpo" , "rumpo2" , "rumpo3" , "speedo" , "speedo2" , "surfer" , "surfer2" , "taco" , "youga" , "airbus" , "airtug" , "brickade" , "bulldozer" , "bus" , "cablecar" , "caddy" , "caddy2" , "coach" , "forklift" , "mower" , "rentalbus" , "ripley" , "tourbus" , "tractor" , "tractor2" , "tractor3" , "trash" , "trash2" , "utillitruck" , "utillitruck2" , "utillitruck3" , "armytanker" , "armytrailer" , "armytrailer2" , "baletrailer" , "boattrailer" , "docktrailer" , "freighttrailer" , "graintrailer" , "proptrailer" , "raketrailer" , "tanker" , "tanker2" , "tr2" , "tr3" , "tr4" , "trailerlogs" , "trailers" , "trailers2" , "trailers3" , "trailersmall" , "trflat" , "tvtrailer" , "freight" , "freightcar" , "freightcont1" , "freightcont2" , "freightgrain" , "metrotrain" , "tankercar" , "policeold2" , "ambulance" , "barracks" , "barracks2" , "fbi" , "riot" , "crusader" , "fbi2" , "lguard" , "pranger" , "policeold1" , "policet" , "sheriff2" , "firetruk" , "pbus" , "rhino" , "police2" , "police" , "police3" , "sheriff" , "police4" , "policeb" , "akuma" , "bagger" , "bati" , "bati2" , "bf400" , "carbonrs" , "cliffhanger" , "daemon" , "double" , "enduro" , "faggio2" , "gargoyle" , "hakuchou" , "hexer" , "innovation" , "lectro" , "nemesis" , "pcj" , "ruffian" , "sanchez" , "sanchez2" , "sovereign" , "thrust" , "vader" , "vindicator" , "bmx" , "cruiser" , "fixter" , "scorcher" , "tribike" , "tribike2" , "tribike3" , "besra" , "cargoplane" , "cuban800" , "dodo" , "duster" , "hydra" , "jet" , "lazer" , "luxor" , "luxor2" , "mammatus" , "miljet" , "nimbus" , "shamal" , "stunt" , "titan" , "velum" , "velum2" , "vestra" , "annihilator" , "blimp" , "blimp2" , "buzzard" , "buzzard2" , "cargobob" , "cargobob2" , "cargobob3" , "cargobob4" , "frogger" , "frogger2" , "maverick" , "polmav" , "savage" , "skylift" , "supervolito" , "supervolito2" , "swift" , "swift2" , "valkyrie" , "valkyrie2" , "volatus" , "dinghy" , "dinghy2" , "dinghy3" , "dinghy4" , "jetmax" , "marquis" , "predator" , "seashark" , "seashark2" , "seashark3" , "speeder" , "speeder2" , "squalo" , "submersible" , "submersible2" , "suntrap" , "toro" , "toro2" , "tropic" , "tropic2" , "tug" , "esskey" , "nightblade" , "defiler" , "avarus" , "zombiea" , "zombieb" , "hakuchou2" , "vortex" , "chimera" , "raptor" , "daemon2" , "ratbike" , "wolfsbane" , "blazer4" , "shotaro" , "tornado6" , "sanctus" , "youga2" , "manchez" , "faggio3" , "faggio" , "youga" };
std::vector<char*> vVehiclesComboList = { "pfister811" , "adder" , "banshee2" , "bullet" , "cheetah" , "entityxf" , "sheava" , "fmj" , "infernus" , "osiris" , "le7b" , "reaper" , "sultanrs" , "t20" , "turismor" , "tyrus" , "vacca" , "voltic" , "prototipo" , "zentorno" , "cogcabrio" , "exemplar" , "f620" , "faction" , "faction2" , "felon" , "felon2" , "jackal" , "sentinel" , "sentinel2" , "windsor" , "windsor2" , "zion" , "zion2" , "alpha" , "banshee" , "bestiagts" , "blista2" , "blista3" , "buffalo" , "buffalo2" , "buffalo3" , "carbonizzare" , "comet2" , "coquette" , "elegy2" , "feltzer2" , "furoregt" , "fusilade" , "futo" , "jester" , "jester2" , "khamelion" , "kuruma" , "kuruma2" , "lynx" , "mamba" , "massacro" , "massacro2" , "ninef" , "ninef2" , "omnis" , "penumbra" , "rapidgt" , "rapidgt2" , "schwarzer" , "seven70" , "sultan" , "surano" , "tropos" , "verlierer2" , "blade" , "buccaneer" , "buccaneer2" , "chino" , "chino2" , "dominator" , "dominator2" , "tampa2" , "dukes" , "dukes2" , "faction3" , "btype2" , "gauntlet" , "gauntlet2" , "hotknife" , "minivan2" , "nightshade" , "phoenix" , "picador" , "ruiner" , "sabregt" , "sabregt2" , "slamvan" , "slamvan2" , "slamvan3" , "stalion" , "stalion2" , "tampa" , "tornado5" , "vigero" , "virgo" , "virgo2" , "virgo3" , "voodoo" , "voodoo2" , "casco" , "coquette2" , "coquette3" , "feltzer3" , "jb700" , "lurcher" , "manana" , "monroe" , "peyote" , "pigalle" , "btype" , "btype3" , "stinger" , "stingergt" , "tornado" , "tornado2" , "tornado3" , "tornado4" , "ztype" , "benson" , "biff" , "boxville" , "boxville2" , "boxville3" , "boxville4" , "cutter" , "docktug" , "dump" , "rallytruck" , "flatbed" , "handler" , "hauler" , "mixer" , "mixer2" , "mule" , "mule2" , "mule3" , "packer" , "phantom" , "pounder" , "rubble" , "scrap" , "stockade" , "stockade3" , "tiptruck" , "tiptruck2" , "towtruck" , "towtruck2" , "asea" , "asea2" , "asterope" , "cog55" , "cog552" , "cognoscenti" , "cognoscenti2" , "emperor" , "emperor2" , "emperor3" , "fugitive" , "glendale" , "ingot" , "intruder" , "limo2" , "oracle" , "oracle2" , "premier" , "primo" , "primo2" , "regina" , "romero" , "schafter2" , "schafter3" , "schafter4" , "schafter5" , "schafter6" , "stanier" , "stratum" , "stretch" , "superd" , "surge" , "tailgater" , "taxi" , "warrener" , "washington" , "bfinjection" , "bifta" , "blazer" , "blazer2" , "blazer3" , "bodhi2" , "brawler" , "trophytruck2" , "dloader" , "dubsta3" , "dune" , "dune2" , "guardian" , "insurgent" , "insurgent2" , "kalahari" , "marshall" , "mesa3" , "monster" , "rancherxl" , "rancherxl2" , "rebel" , "rebel2" , "sandking" , "sandking2" , "technical" , "trophytruck" , "baller" , "baller2" , "baller3" , "baller4" , "baller5" , "baller6" , "bjxl" , "cavalcade" , "cavalcade2" , "dubsta" , "dubsta2" , "fq2" , "granger" , "gresley" , "habanero" , "huntley" , "landstalker" , "mesa" , "mesa2" , "patriot" , "radi" , "rocoto" , "seminole" , "serrano" , "xls" , "xls2" , "blista" , "brioso" , "dilettante" , "dilettante2" , "issi2" , "panto" , "prairie" , "rhapsody" , "bison" , "bison2" , "bison3" , "bobcatxl" , "contender" , "ratloader" , "ratloader2" , "sadler" , "sadler2" , "burrito" , "burrito2" , "burrito3" , "burrito4" , "burrito5" , "camper" , "gburrito" , "gburrito2" , "journey" , "minivan" , "moonbeam" , "moonbeam2" , "paradise" , "pony" , "pony2" , "rumpo" , "rumpo2" , "rumpo3" , "speedo" , "speedo2" , "surfer" , "surfer2" , "taco" , "youga" , "airbus" , "airtug" , "brickade" , "bulldozer" , "bus" , "cablecar" , "caddy" , "caddy2" , "coach" , "forklift" , "mower" , "rentalbus" , "ripley" , "tourbus" , "tractor" , "tractor2" , "tractor3" , "trash" , "trash2" , "utillitruck" , "utillitruck2" , "utillitruck3" , "armytanker" , "armytrailer" , "armytrailer2" , "baletrailer" , "boattrailer" , "docktrailer" , "freighttrailer" , "graintrailer" , "proptrailer" , "raketrailer" , "tanker" , "tanker2" , "tr2" , "tr3" , "tr4" , "trailerlogs" , "trailers" , "trailers2" , "trailers3" , "trailersmall" , "trflat" , "tvtrailer" , "freight" , "freightcar" , "freightcont1" , "freightcont2" , "freightgrain" , "metrotrain" , "tankercar" , "policeold2" , "ambulance" , "barracks" , "barracks2" , "fbi" , "riot" , "crusader" , "fbi2" , "lguard" , "pranger" , "policeold1" , "policet" , "sheriff2" , "firetruk" , "pbus" , "rhino" , "police2" , "police" , "police3" , "sheriff" , "police4" , "policeb" , "akuma" , "bagger" , "bati" , "bati2" , "bf400" , "carbonrs" , "cliffhanger" , "daemon" , "double" , "enduro" , "faggio2" , "gargoyle" , "hakuchou" , "hexer" , "innovation" , "lectro" , "nemesis" , "pcj" , "ruffian" , "sanchez" , "sanchez2" , "sovereign" , "thrust" , "vader" , "vindicator" , "bmx" , "cruiser" , "fixter" , "scorcher" , "tribike" , "tribike2" , "tribike3" , "besra" , "cargoplane" , "cuban800" , "dodo" , "duster" , "hydra" , "jet" , "lazer" , "luxor" , "luxor2" , "mammatus" , "miljet" , "nimbus" , "shamal" , "stunt" , "titan" , "velum" , "velum2" , "vestra" , "annihilator" , "blimp" , "blimp2" , "buzzard" , "buzzard2" , "cargobob" , "cargobob2" , "cargobob3" , "cargobob4" , "frogger" , "frogger2" , "maverick" , "polmav" , "savage" , "skylift" , "supervolito" , "supervolito2" , "swift" , "swift2" , "valkyrie" , "valkyrie2" , "volatus" , "dinghy" , "dinghy2" , "dinghy3" , "dinghy4" , "jetmax" , "marquis" , "predator" , "seashark" , "seashark2" , "seashark3" , "speeder" , "speeder2" , "squalo" , "submersible" , "submersible2" , "suntrap" , "toro" , "toro2" , "tropic" , "tropic2" , "tug" , "esskey" , "nightblade" , "defiler" , "avarus" , "zombiea" , "zombieb" , "hakuchou2" , "vortex" , "chimera" , "raptor" , "daemon2" , "ratbike" , "wolfsbane" , "blazer4" , "shotaro" , "tornado6" , "sanctus" , "youga2" , "manchez" , "faggio3" , "faggio" , "youga" };

DWORD lasttime;

void Script::onTick()
{
	Player playerid = PLAYER::PLAYER_ID();
	Ped player = PLAYER::GET_PLAYER_PED(playerid);
	Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(player, false);
	Vector3 location = ENTITY::GET_ENTITY_COORDS(player, ENTITY::IS_ENTITY_DEAD(player));

	if (!settime) {
		jl_StatCash_t = timeGetTime();
		settime = true;
	}

	for (int i = 0; i < MAX_PLAYERS + 1; i++)
	{
		players[i].id = i;
		players[i].ped = PLAYER::GET_PLAYER_PED(i);

		players[i].name = PLAYER::GET_PLAYER_NAME(i);
		players[i].health = ENTITY::GET_ENTITY_HEALTH(players[i].ped);
		players[i].maxhealth = ENTITY::GET_ENTITY_MAX_HEALTH(players[i].ped);

		players[i].coordinates = ENTITY::GET_ENTITY_COORDS(players[i].ped, ENTITY::IS_ENTITY_DEAD(players[i].ped));
	}

	ENTITY::SET_ENTITY_VISIBLE(player, jl_Visible, false);
	ENTITY::SET_ENTITY_VISIBLE(veh, jl_VVisible, false);

	if (jl_NeverWanted && PLAYER::GET_PLAYER_WANTED_LEVEL(playerid) != 0)
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(playerid);

	if (jl_GodMode) {
		ENTITY::SET_ENTITY_INVINCIBLE(player, jl_GodMode_t);
		jl_GodMode = false;
	}

	if (PED::CAN_PED_RAGDOLL(player) != !jl_DisableRagdoll)
		PED::SET_PED_CAN_RAGDOLL(player, !jl_DisableRagdoll);

	if (jl_Suicide) {
		ENTITY::SET_ENTITY_HEALTH(player, 0);
		jl_Suicide = false;
	}

	if (jl_SuperJump)
		GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(playerid);

	if (jl_FastRun) {
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(playerid, jl_FastRun_f);
		jl_FastRun = false;
	}

	if (jl_FastSwim) {
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(playerid, jl_FastSwim_f);
		jl_FastSwim = false;
	}

	if (jl_FireAmmo)
			GAMEPLAY::SET_FIRE_AMMO_THIS_FRAME(playerid);

	if (jl_ExplosiveAmmo)
			GAMEPLAY::SET_EXPLOSIVE_AMMO_THIS_FRAME(playerid);

	if (jl_ExplosiveMelee)
			GAMEPLAY::SET_EXPLOSIVE_MELEE_THIS_FRAME(playerid);

	if (jl_StatRP) {
		if (PLAYER::GET_PLAYER_WANTED_LEVEL(playerid) == 5) {
			WAIT(jl_StatRP_d);
			PLAYER::SET_PLAYER_WANTED_LEVEL(playerid, 0, false);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(playerid, false);
		} else {
			if (PLAYER::GET_PLAYER_WANTED_LEVEL(playerid) == 0) {
				PLAYER::SET_PLAYER_WANTED_LEVEL(playerid, 5, false);
				PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(playerid, false);
				PLAYER::SET_PLAYER_WANTED_CENTRE_POSITION(playerid, 1391.773f, 3608.716f, 38.942f);
			}
		}
	}

	if (jl_StatCash)
	{
		if ((timeGetTime() - jl_StatCash_t) > jl_StatCash_d)
			OBJECT::CREATE_AMBIENT_PICKUP(GAMEPLAY::GET_HASH_KEY("PICKUP_MONEY_CASE"), location.x, location.y, location.z+1.0f, 0, 2000, 1, 0, 1);
		jl_StatCash_t = timeGetTime();
	}

	if (jl_StatOtherCash) {
		OBJECT::CREATE_AMBIENT_PICKUP(GAMEPLAY::GET_HASH_KEY("PICKUP_MONEY_CASE"), players[jl_StatOtherCashTo].coordinates.x, players[jl_StatOtherCashTo].coordinates.y, players[jl_StatOtherCashTo].coordinates.z + 1.0f, 0, 2000, 1, 0, 1);
	}

	if (jl_Yankton)
	{
		STREAMING::REQUEST_IPL("plg_01");
		STREAMING::REQUEST_IPL("prologue01");
		STREAMING::REQUEST_IPL("prologue01_lod");
		STREAMING::REQUEST_IPL("prologue01c");
		STREAMING::REQUEST_IPL("prologue01c_lod");
		STREAMING::REQUEST_IPL("prologue01d");
		STREAMING::REQUEST_IPL("prologue01d_lod");
		STREAMING::REQUEST_IPL("prologue01e");
		STREAMING::REQUEST_IPL("prologue01e_lod");
		STREAMING::REQUEST_IPL("prologue01f");
		STREAMING::REQUEST_IPL("prologue01f_lod");
		STREAMING::REQUEST_IPL("prologue01g");
		STREAMING::REQUEST_IPL("prologue01h");
		STREAMING::REQUEST_IPL("prologue01h_lod");
		STREAMING::REQUEST_IPL("prologue01i");
		STREAMING::REQUEST_IPL("prologue01i_lod");
		STREAMING::REQUEST_IPL("prologue01j");
		STREAMING::REQUEST_IPL("prologue01j_lod");
		STREAMING::REQUEST_IPL("prologue01k");
		STREAMING::REQUEST_IPL("prologue01k_lod");
		STREAMING::REQUEST_IPL("prologue01z");
		STREAMING::REQUEST_IPL("prologue01z_lod");
		STREAMING::REQUEST_IPL("plg_02");
		STREAMING::REQUEST_IPL("prologue02");
		STREAMING::REQUEST_IPL("prologue02_lod");
		STREAMING::REQUEST_IPL("plg_03");
		STREAMING::REQUEST_IPL("prologue03");
		STREAMING::REQUEST_IPL("prologue03_lod");
		STREAMING::REQUEST_IPL("prologue03b");
		STREAMING::REQUEST_IPL("prologue03b_lod");
		STREAMING::REQUEST_IPL("prologue03_grv_dug");
		STREAMING::REQUEST_IPL("prologue03_grv_dug_lod");
		STREAMING::REQUEST_IPL("prologue_grv_torch");
		STREAMING::REQUEST_IPL("plg_04");
		STREAMING::REQUEST_IPL("prologue04");
		STREAMING::REQUEST_IPL("prologue04_lod");
		STREAMING::REQUEST_IPL("prologue04b");
		STREAMING::REQUEST_IPL("prologue04b_lod");
		STREAMING::REQUEST_IPL("prologue04_cover");
		STREAMING::REQUEST_IPL("des_protree_end");
		STREAMING::REQUEST_IPL("des_protree_start");
		STREAMING::REQUEST_IPL("des_protree_start_lod");
		STREAMING::REQUEST_IPL("plg_05");
		STREAMING::REQUEST_IPL("prologue05");
		STREAMING::REQUEST_IPL("prologue05_lod");
		STREAMING::REQUEST_IPL("prologue05b");
		STREAMING::REQUEST_IPL("prologue05b_lod");
		STREAMING::REQUEST_IPL("plg_06");
		STREAMING::REQUEST_IPL("prologue06");
		STREAMING::REQUEST_IPL("prologue06_lod");
		STREAMING::REQUEST_IPL("prologue06b");
		STREAMING::REQUEST_IPL("prologue06b_lod");
		STREAMING::REQUEST_IPL("prologue06_int");
		STREAMING::REQUEST_IPL("prologue06_int_lod");
		STREAMING::REQUEST_IPL("prologue06_pannel");
		STREAMING::REQUEST_IPL("prologue06_pannel_lod");
		STREAMING::REQUEST_IPL("prologue_m2_door");
		STREAMING::REQUEST_IPL("prologue_m2_door_lod");
		STREAMING::REQUEST_IPL("plg_occl_00");
		STREAMING::REQUEST_IPL("prologue_occl");
		STREAMING::REQUEST_IPL("plg_rd");
		STREAMING::REQUEST_IPL("prologuerd");
		STREAMING::REQUEST_IPL("prologuerdb");
		STREAMING::REQUEST_IPL("prologuerd_lod");

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, 3360.19f, -4849.67f, 111.8f, false, false, false);
		jl_Yankton = false;
	}

	if (jl_TeleportPlayerHere) {
		int telhere = jl_TeleportHere;
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(player, ENTITY::IS_ENTITY_DEAD(player));
		AI::TASK_FOLLOW_NAV_MESH_TO_COORD(players[telhere].ped, coords.x, coords.y, coords.z, 1, -1, 0, 0, 0);

		STREAMING::REQUEST_ANIM_DICT("nm@stunt_jump");

		while (!STREAMING::HAS_ANIM_DICT_LOADED("nm@stunt_jump"))
			WAIT(0);
		
		int sceneID = NETWORK::NETWORK_CREATE_SYNCHRONISED_SCENE(coords.x, coords.y, coords.z, 0, 0, 0, 2, 0, 1, 1.0f);
		NETWORK::NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE(players[telhere].ped, sceneID, "nm@stunt_jump", "jump_loop", 8.0f, -8.0f, 120, 0, 100, 0);
		NETWORK::NETWORK_START_SYNCHRONISED_SCENE(sceneID);//will start the animation
		AI::CLEAR_PED_TASKS(players[telhere].ped);

		jl_TeleportPlayerHere = false;
	}

	if (jl_TeleportToPlayer) {
		int telto = jl_TeleportTo;
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, players[telto].coordinates.x+2.0f, players[telto].coordinates.y, players[telto].coordinates.z, false, false, false);
		jl_TeleportToPlayer = false;
	}

	if (jl_CreateVehicle) {
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(player, ENTITY::IS_ENTITY_DEAD(player));
		Hash vehicle = GAMEPLAY::GET_HASH_KEY(vVehiclesComboList[jl_CreateVehicleId]);
		VEHICLE::CREATE_VEHICLE(vehicle, coords.x, coords.y + 1.0f, coords.z + 1.0f, ENTITY::GET_ENTITY_HEADING(player), true, true);

		jl_CreateVehicle = false;
	}

	if (jl_SpeedUpVehicle) {
		VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, VEHICLE::_GET_VEHICLE_MODEL_MAX_SPEED(ENTITY::GET_ENTITY_MODEL(veh)));

		jl_SpeedUpVehicle = false;
	}

	if (jl_ReduceGrip_Set) {
		VEHICLE::SET_VEHICLE_REDUCE_GRIP(veh, jl_ReduceGrip);
	}

	if (jl_RepairVehicle) {
		VEHICLE::SET_VEHICLE_FIXED(veh);
		VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(veh);

		VEHICLE::SET_VEHICLE_BODY_HEALTH(veh, 1000.0f);
		VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 1000.0f);
		VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(veh, 1000.0f);

		jl_RepairVehicle = false;
	}
}

void Script::dxTick()
{
	ImGuiIO& io = ImGui::GetIO();

	Player playerid = PLAYER::PLAYER_ID();
	Ped player = PLAYER::GET_PLAYER_PED(playerid);

	if (showjlmenu) {
		io.MouseDrawCursor = true;

		ImGui::SetNextWindowSize(ImVec2(550, 300), ImGuiSetCond_FirstUseEver);
		ImGui::Begin(" jLmenu", &showjlmenu, ImGuiWindowFlags_NoSavedSettings);

		// Player
		if (ImGui::CollapsingHeader("Player"))
		{
			ImGui::Text("General");
			ImGui::Checkbox("Never Wanted", &jl_NeverWanted);
			ImGui::Checkbox("Disable Ragdoll", &jl_DisableRagdoll);

			ImGui::Separator();

			ImGui::Text("Health");
			if (ImGui::Checkbox("God Mode", &jl_GodMode_t)) jl_GodMode = true;
			ImGui::Checkbox("Visible", &jl_Visible);
			if (ImGui::Button("Suicide")) jl_Suicide = true;

			ImGui::Separator();

			ImGui::Text("Movement");
			ImGui::Checkbox("Super Jump", &jl_SuperJump);
			if (ImGui::SliderFloat("Running Speed", &jl_FastRun_f, 1.0f, 1.49f))
				jl_FastRun = true;
			if (ImGui::SliderFloat("Swimming Speed", &jl_FastSwim_f, 1.0f , 1.49f))
				jl_FastSwim = true;

			ImGui::Separator();

			ImGui::Text("Weapons");
			ImGui::Checkbox("Fire Ammo", &jl_FireAmmo);
			ImGui::Checkbox("Explosive Ammo", &jl_ExplosiveAmmo);
			ImGui::Checkbox("Explosive Melee", &jl_ExplosiveMelee);

			ImGui::Separator();

			ImGui::Text("Statistics");
			if (ImGui::TreeNode("RP")) {
				ImGui::Checkbox("Enable", &jl_StatRP);
				ImGui::SliderInt("Delay", &jl_StatRP_d, 10, 500);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Money")) {
				ImGui::Checkbox("Enable", &jl_StatCash);
				ImGui::SliderInt("Delay", &jl_StatCash_d, 0, 1000);
			}
		}

		// Vehicle
		if (ImGui::CollapsingHeader("Vehicle"))
		{
			ImGui::Combo("##createvehicle", &jl_CreateVehicleId, VehiclesComboList, IM_ARRAYSIZE(VehiclesComboList));
			ImGui::SameLine();
			if (ImGui::Button("Spawn")) jl_CreateVehicle = true;
			ImGui::NewLine();
			if (ImGui::Button("Speed Up")) jl_SpeedUpVehicle = true;
			if (ImGui::Button("Repair Vehicle")) jl_RepairVehicle = true;
			if (ImGui::Checkbox("Reduce Grip", &jl_ReduceGrip))
				jl_ReduceGrip_Set = true;
			ImGui::Checkbox("Visible", &jl_VVisible);
		}

		// Teleporting
		if (ImGui::CollapsingHeader("Teleport"))
		{
			if (ImGui::Button("North Yankton")) jl_Yankton = true;
		}

		// Online Players
		if (ImGui::CollapsingHeader("Online Players"))
		{
			for (int i = 0; i < MAX_PLAYERS + 1; i++)
			{
				if (players[i].maxhealth > 0.0f) {
					if (ImGui::TreeNode(players[i].name))
					{
						ImGui::Text("Health: %f(%f)", players[i].health, players[i].maxhealth);
						ImGui::Text("Coordinates: %f, %f, %f", players[i].coordinates.x, players[i].coordinates.y, players[i].coordinates.z);

						if(ImGui::Button("Teleport")) {
							jl_TeleportTo = i;
							jl_TeleportToPlayer = true;
						}
						ImGui::SameLine();
						if (ImGui::Button("Bring")) {
							jl_TeleportHere = i;
							jl_TeleportPlayerHere = true;
						}
						ImGui::SameLine();
						if (ImGui::Button("Money bags (2k)")) {
							jl_StatOtherCashTo = i;
							jl_StatOtherCash = !jl_StatOtherCash;
						}
						ImGui::TreePop();
					}
				}
			}
		}

		ImGui::SetWindowFocus();
		ImGui::End();
	}
}
