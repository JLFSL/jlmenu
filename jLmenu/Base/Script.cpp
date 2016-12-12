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

#define MAX_PLAYERS 32
struct Players {
	int id;
	Ped ped;

	char *name;
	int health;
	int maxhealth;

	Vector3 coordinates;
} players[1000];

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
static bool jl_StatCash = false;

static bool jl_SpeedUpVehicle = false;
static bool jl_RepairVehicle = false;
static bool jl_ReduceGrip = false;
static bool jl_ReduceGrip_Set = false;
static bool jl_VVisible = true;

static bool jl_TeleportToPlayer = false;
static int jl_TeleportTo = 0;

DWORD lasttime;

static int statuc;

void Script::onTick()
{
	Player playerid = PLAYER::PLAYER_ID();
	Ped player = PLAYER::GET_PLAYER_PED(playerid);
	Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(player, false);
	Vector3 location = ENTITY::GET_ENTITY_COORDS(player, ENTITY::IS_ENTITY_DEAD(player));

	for (int i = 0; i < MAX_PLAYERS + 1; i++)
	{
		players[i].id = i;
		players[i].ped = PLAYER::GET_PLAYER_PED(i);

		players[i].name = PLAYER::GET_PLAYER_NAME(i);
		players[i].health = ENTITY::GET_ENTITY_HEALTH(players[i].ped);
		players[i].maxhealth = ENTITY::GET_ENTITY_MAX_HEALTH(players[i].ped);

		
		Vector3 tempc = ENTITY::GET_ENTITY_COORDS(players[i].ped, ENTITY::IS_ENTITY_DEAD(players[i].ped));
		players[i].coordinates.x = tempc.x;
		players[i].coordinates.y = tempc.y;
		players[i].coordinates.z = tempc.z;
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
			WAIT(200);
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
		OBJECT::CREATE_MONEY_PICKUPS(location.x, location.y, location.z+1.0f, 2000, 1, GAMEPLAY::GET_HASH_KEY("prop_money_bag_01"));
	}

	if (jl_TeleportToPlayer) {
		int telto = jl_TeleportTo;
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, players[telto].coordinates.x+2.0f, players[telto].coordinates.y, players[telto].coordinates.z, false, false, false);
		jl_TeleportToPlayer = false;
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

bool showjlmenu = true;

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
			ImGui::Checkbox("RP Cheat", &jl_StatRP);
			ImGui::Checkbox("Money bags (dont use)", &jl_StatCash);
		}
		// Vehicle
		if (ImGui::CollapsingHeader("Vehicle"))
		{
			if (ImGui::Button("Speed Up")) jl_SpeedUpVehicle = true;
			if (ImGui::Button("Repair Vehicle")) jl_RepairVehicle = true;
			if (ImGui::Checkbox("Reduce Grip", &jl_ReduceGrip))
				jl_ReduceGrip_Set = true;
			ImGui::Checkbox("Visible", &jl_VVisible);
		}

		// Online Players
		if (ImGui::CollapsingHeader("Online Players"))
		{
			//ImGui::BeginChild("Players");
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
						ImGui::TreePop();
					}
				}
			}
			//ImGui::EndChild();
		}

		ImGui::SetWindowFocus();
		ImGui::End();
	}
}
