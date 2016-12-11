#include "stdafx.h"

// DX11Renderer.cpp
DWORD __stdcall InitializeHook(LPVOID);
void* detourBuffer[3];

bool Script::isInit()
{
	CreateThread(NULL, NULL, InitializeHook, NULL, 0, NULL);

	return true;
}

void Script::onTick()
{
	Player playerid = PLAYER::PLAYER_ID();
	Ped player = PLAYER::GET_PLAYER_PED(playerid);

	if (KeyJustUp(VK_F8)) {
		Vector3 CurrentCoordinates = ENTITY::GET_ENTITY_COORDS(player, ENTITY::IS_ENTITY_DEAD(player));

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(player, CurrentCoordinates.x, CurrentCoordinates.y, CurrentCoordinates.z+3.0f, false, false, false);
	}
}
