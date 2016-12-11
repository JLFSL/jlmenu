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
#include "BeaEngine.h"
#include "DX11Renderer.h"