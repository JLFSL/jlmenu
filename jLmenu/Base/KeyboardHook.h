#pragma once

class KeyboardHook {
public:
	//single ton
	static KeyboardHook& Instance() {
		static KeyboardHook keyboardHook;
		return keyboardHook;
	}
	HHOOK hook;
	void InstallHook();
	void UninstallHook();

	MSG msg;
	int Messages();
};
LRESULT __stdcall HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //callback declaration

