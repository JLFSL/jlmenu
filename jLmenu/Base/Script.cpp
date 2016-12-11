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

static bool jl_NeverWanted = false;
static bool jl_GodMode = false;
static bool jl_DisableRagdoll = false;
static bool jl_Suicide = false;
static bool jl_SuperJump = false;
static bool jl_FastRun = false;
static float jl_FastRun_f = false;
static bool jl_FastSwim = false;
static float jl_FastSwim_f = false;
static bool jl_FireAmmo = false;
static bool jl_ExplosiveAmmo = false;
static bool jl_ExplosiveMelee = false;

void Script::onTick()
{
	Player playerid = PLAYER::PLAYER_ID();
	Ped player = PLAYER::GET_PLAYER_PED(playerid);

	if (sd_KeyJustUp(VK_F8)) {
		Vector3 CurrentCoordinates = ENTITY::GET_ENTITY_COORDS(player, ENTITY::IS_ENTITY_DEAD(player));

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, CurrentCoordinates.x, CurrentCoordinates.y, CurrentCoordinates.z+3.0f, false, false, false);
	}

	if (jl_NeverWanted && PLAYER::GET_PLAYER_WANTED_LEVEL(playerid) != 0)
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(playerid);

	if (jl_GodMode)
		ENTITY::SET_ENTITY_HEALTH(player, ENTITY::GET_ENTITY_MAX_HEALTH(player) - 1.0);

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
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(playerid, jl_FastSwim_f);
		jl_FastSwim = false;
	}

	if (jl_FireAmmo)
			GAMEPLAY::SET_FIRE_AMMO_THIS_FRAME(playerid);

	if (jl_ExplosiveAmmo)
			GAMEPLAY::SET_EXPLOSIVE_AMMO_THIS_FRAME(playerid);

	if (jl_ExplosiveMelee)
			GAMEPLAY::SET_EXPLOSIVE_MELEE_THIS_FRAME(playerid);

}

bool showjlmenu = true;

void Script::dxTick()
{
	ImGuiIO& io = ImGui::GetIO();

	Player playerid = PLAYER::PLAYER_ID();
	Ped player = PLAYER::GET_PLAYER_PED(playerid);

	if (showjlmenu) {
		io.MouseDrawCursor = true;

		ImGui::SetNextWindowPos(ImVec2(100, 100));
		ImGui::SetNextWindowSize(ImVec2(550, 300), ImGuiSetCond_FirstUseEver);
		ImGui::Begin(" jLmenu", &showjlmenu, ImGuiWindowFlags_NoSavedSettings);

		ImGui::PushItemWidth(-140);

		// Player
		if (ImGui::CollapsingHeader("Player Options"))
		{
			ImGui::Text("General");
			ImGui::Checkbox("Never Wanted", &jl_NeverWanted);
			ImGui::Checkbox("Disable Ragdoll", &jl_DisableRagdoll);

			ImGui::Separator();

			ImGui::Text("Health");
			ImGui::Checkbox("God Mode", &jl_GodMode);
			if (ImGui::Button("Suicide")) jl_Suicide = true;

			ImGui::Separator();

			ImGui::Text("Movement");
			ImGui::Checkbox("Super Jump", &jl_SuperJump);
			if (ImGui::SliderFloat("Running Speed", &jl_FastRun_f, 0.0f, 1.49f))
				jl_FastRun = true;
			if (ImGui::SliderFloat("Swimming Speed", &jl_FastSwim_f, 0.0f, 1.49f))
				jl_FastSwim = true;

			ImGui::Separator();

			ImGui::Text("Weapons");
			ImGui::Checkbox("Fire Ammo", &jl_FireAmmo);
			ImGui::Checkbox("Explosive Ammo", &jl_ExplosiveAmmo);
			ImGui::Checkbox("Explosive Melee", &jl_ExplosiveMelee);
		}

		ImGui::SetWindowFocus();
		ImGui::End();
	}
}
