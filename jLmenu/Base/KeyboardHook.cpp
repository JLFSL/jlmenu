#include "stdafx.h"

WNDPROC OldWndProc = nullptr;

void KeyboardHook::InstallHook() {
	OldWndProc = (WNDPROC)SetWindowLongPtr(FindWindowA(0, "Grand Theft Auto V"), GWLP_WNDPROC, (LONG_PTR)HookedWndProc);
}

void KeyboardHook::UninstallHook() {
	UnhookWindowsHookEx(hook);
}

int KeyboardHook::Messages() {
	while (msg.message != WM_QUIT) {
		BlockInput(false);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}
	UninstallHook();
	return (int)msg.wParam;
}

LRESULT __stdcall HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (uMsg) {
	case WM_KEYDOWN: {
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
	}break;

	case WM_KEYUP: {
		if (wParam < 256)
			io.KeysDown[wParam] = 0;

		if (wParam == VK_F5)
			showjlmenu = !showjlmenu;

	}break;

	case WM_CHAR: {
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
	}break;
	}

	return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
}