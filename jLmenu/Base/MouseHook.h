#pragma once

class MouseHook {
public:
	static MouseHook& Instance() {
		static MouseHook mouseHook;
		return mouseHook;
	}
	HHOOK hook;
	void InstallHook();
	void UninstallHook();

	MSG msg;
	int Messages();
	void ForegroundCheck();

	bool IsForeGround = true;
};
LRESULT WINAPI MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam);

