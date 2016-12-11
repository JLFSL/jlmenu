#include "stdafx.h"

void MouseHook::InstallHook() {
	hook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0);
}

void MouseHook::UninstallHook() {
	UnhookWindowsHookEx(hook);
}

int MouseHook::Messages() {
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}
	UninstallHook();
	return (int)msg.wParam;
}

bool callbackforeground = true;

void MouseHook::ForegroundCheck() {
	while (msg.message != WM_QUIT) {
		HWND forgnd = GetForegroundWindow();

		int bufferLen = GetWindowTextLength(forgnd) + 1;
		std::string wnd_ftitle(bufferLen, '\0');
		GetWindowTextA(forgnd, &wnd_ftitle[0], bufferLen);

		if (std::strcmp((char*)wnd_ftitle.c_str(), "Grand Theft Auto V") == 0) {
			if (!IsForeGround)
				IsForeGround = true;
		}
		else {
			if (IsForeGround)
				IsForeGround = false;
		}

		callbackforeground = IsForeGround;

		Sleep(100); //10th of a second sleep as we don't need to check that frequently
	}
	return;
}

LRESULT WINAPI MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == 0 && callbackforeground) {
		ImGuiIO& io = ImGui::GetIO();
		switch (wParam) {
		case WM_KEYDOWN: {
			if (wParam < 256)
				io.KeysDown[wParam] = 1;
		}break;

		case WM_KEYUP: {
			if (wParam < 256)
				io.KeysDown[wParam] = 0;
		}break;

		case WM_CHAR: {
			if (wParam > 0 && wParam < 0x10000)
				io.AddInputCharacter((unsigned short)wParam);
		}break;
		case WM_LBUTTONUP: {
			io.MouseDown[0] = false;
		}break;

		case WM_LBUTTONDOWN: {
			io.MouseDown[0] = true;
		}break;

		case WM_RBUTTONDOWN: {
			io.MouseDown[1] = true;
		}break;

		case WM_RBUTTONUP: {
			io.MouseDown[1] = false;
		}break;

		case WM_MBUTTONDOWN: {
			io.MouseDown[2] = true;
		}break;

		case WM_MBUTTONUP: {
			io.MouseDown[2] = false;
		}break;

		case WM_MOUSEWHEEL: {
			MSLLHOOKSTRUCT *pMhs = (MSLLHOOKSTRUCT *)lParam;
			short zDelta = HIWORD(pMhs->mouseData);

			if (zDelta > 0) {
				io.MouseWheel += 0.2f;
			}
			else {
				io.MouseWheel -= 0.2f;
			}
		}break;

		case WM_MOUSEMOVE: {
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
		}break;
		}
	}
	return CallNextHookEx(MouseHook::Instance().hook, nCode, wParam, lParam);
}