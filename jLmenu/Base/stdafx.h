#pragma once

// Windows
#include <winsock2.h>
#include <Windows.h>
#include <stdint.h>
#include <time.h>

// C++
#include <cstdio>
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <set>
using namespace std;

// Shared
#include "../../Shared/Common.h"

// DirectX 11
#include <d3d11_2.h>

// MinHook
#include "../../Vendor/MinHook/include/MinHook.h"

// SudoMod
#include "Logger.h"
#include "Memory.h"
#include "types.h"
#include "enums.h"
#include "InputHook.h"
#include "keyboard.h"
#include "CrossMapping.h"
#include "NativeInvoker.h"
#include "nativeCaller.h"
#include "natives.h"
#include "Hooking.h"
#include "Structs.h"
#include "script.h"

// DirectX
#include "DX11Renderer.h"

// BeaEngine
#include "BeaEngine.h"

// ImGui
#include "imgui.h"
#include "imgui_impl_dx11.h"