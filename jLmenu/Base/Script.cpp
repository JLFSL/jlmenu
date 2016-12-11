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
	//CreateThread(NULL, 0, KeyboardHook, NULL, 0, NULL);
	CreateThread(NULL, 0, InitializeHook, NULL, 0, NULL);
	//CreateThread(NULL, 0, ForegroundCheck, NULL, 0, NULL);
	return true;
}

void Script::onTick()
{
	Player playerid = PLAYER::PLAYER_ID();
	Ped player = PLAYER::GET_PLAYER_PED(playerid);

	if (sd_KeyJustUp(VK_F8)) {
		Vector3 CurrentCoordinates = ENTITY::GET_ENTITY_COORDS(player, ENTITY::IS_ENTITY_DEAD(player));

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, CurrentCoordinates.x, CurrentCoordinates.y, CurrentCoordinates.z+3.0f, false, false, false);
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

		ImGui::SetNextWindowPos(ImVec2(100, 100));
		ImGui::SetNextWindowSize(ImVec2(300, 200));
		ImGui::Begin(" jLmenu", &showjlmenu, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

		static int jl_WantedLevel = 0;
		ImGui::SliderInt("Wanted Level", &jl_WantedLevel, 0, 5);

		int wantedlevel = PLAYER::GET_PLAYER_WANTED_LEVEL(playerid);
		if (wantedlevel != jl_WantedLevel) {
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, jl_WantedLevel, false);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, false);

			Logger::Msg("PLAYER:: WANTED to %d", jl_WantedLevel);
		}

		ImGui::End();
	}
}
