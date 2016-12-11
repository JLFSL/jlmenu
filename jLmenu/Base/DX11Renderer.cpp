#include "stdafx.h"

HWND hWnd;
bool firstTime = true;

HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
void __stdcall hookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
void __stdcall hookD3D11ClearRenderTargetView(ID3D11DeviceContext* pContext, ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4]);

// Hook Calls
typedef HRESULT(__stdcall *D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall *D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall *D3D11ClearRenderTargetViewHook) (ID3D11DeviceContext* pContext, ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4]);

// GTA 5's dx11 device/devicecontext
ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL;

// Swapchain to retrieve above
DWORD_PTR* pSwapChainVtable = NULL;
DWORD_PTR* pDeviceContextVTable = NULL;

// more dx11
D3D11PresentHook phookD3D11Present = NULL;
D3D11DrawIndexedHook phookD3D11DrawIndexed = NULL;
D3D11ClearRenderTargetViewHook phookD3D11ClearRenderTargetView = NULL;

// Hook Functions
const void* DetourFunc64(BYTE* const src, const BYTE* dest, const unsigned int jumplength);
const unsigned int DisasmLengthCheck(const SIZE_T address, const unsigned int jumplength);
const void* DetourFuncVTable(SIZE_T* src, const BYTE* dest, const DWORD index);

// Hook Vars
struct HookContext
{
	BYTE original_code[64];
	SIZE_T dst_ptr;
	BYTE far_jmp[6];
};
HookContext* presenthook64;

HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (firstTime)
	{
		pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
		pDevice->GetImmediateContext(&pContext);

		ImGui_ImplDX11_Init(hWnd, pDevice, pContext);

		{
			ImGuiStyle& style = ImGui::GetStyle();
			bool bStyleDark_ = true;
			float alpha_ = 1.0;

			// light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
			style.Alpha = 1.0f;
			style.FrameRounding = 1.0f;
			style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
			style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
			style.Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
			style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
			style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
			style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
			style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

			if (bStyleDark_)
			{
				for (int i = 0; i < ImGuiCol_COUNT; i++)
				{
					ImVec4& col = style.Colors[i];
					float H, S, V;
					ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

					if (S < 0.1f)
					{
						V = 1.0f - V;
					}
					ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
					if (col.w < 1.00f)
					{
						col.w *= alpha_;
					}
				}
			}
			else
			{
				for (int i = 0; i < ImGuiCol_COUNT; i++)
				{
					ImVec4& col = style.Colors[i];
					if (col.w < 1.00f)
					{
						col.x *= alpha_;
						col.y *= alpha_;
						col.z *= alpha_;
						col.w *= alpha_;
					}
				}
			}

			firstTime = false;
		}

	}
	ImGui_ImplDX11_NewFrame();

	Script::dxTick();
	
	ImGui::Render();

	return phookD3D11Present(pSwapChain, SyncInterval, Flags);
}

void __stdcall hookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	return phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}

void __stdcall hookD3D11ClearRenderTargetView(ID3D11DeviceContext* pContext, ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4])
{
	return phookD3D11ClearRenderTargetView(pContext, pRenderTargetView, ColorRGBA);
}

DWORD __stdcall InitializeHook(LPVOID)
{
	hWnd = FindWindowA(NULL, "Grand Theft Auto V");
	IDXGISwapChain* pSwapChain;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE;//((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE; 
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, NULL, &pContext)))
	{
		Logger::Msg("DX11: Failed");
		return NULL;
	}
	Logger::Msg("DX11: Success");

	pSwapChainVtable = (DWORD_PTR*)pSwapChain;
	pSwapChainVtable = (DWORD_PTR*)pSwapChainVtable[0];

	pDeviceContextVTable = (DWORD_PTR*)pContext;
	pDeviceContextVTable = (DWORD_PTR*)pDeviceContextVTable[0];

	/*if (Config->client_steam_def) {
		phookD3D11Present = (D3D11PresentHook)DetourFunc64((BYTE*)pSwapChainVtable[8] + 0x5, (BYTE*)hookD3D11Present, 16);
	}
	else {*/
		phookD3D11Present = (D3D11PresentHook)DetourFunc64((BYTE*)pSwapChainVtable[8], (BYTE*)hookD3D11Present, 16);
	//}

	phookD3D11DrawIndexed = (D3D11DrawIndexedHook)DetourFunc64((BYTE*)pDeviceContextVTable[12], (BYTE*)hookD3D11DrawIndexed, 16);
	phookD3D11ClearRenderTargetView = (D3D11ClearRenderTargetViewHook)DetourFunc64((BYTE*)pDeviceContextVTable[50], (BYTE*)hookD3D11ClearRenderTargetView, 16);

	pDevice->Release();
	pContext->Release();
	pSwapChain->Release();

	return NULL;
}

const void* DetourFunc64(BYTE* const src, const BYTE* dest, const unsigned int jumplength)
{
	// Allocate a memory page that is going to contain executable code. 
	MEMORY_BASIC_INFORMATION mbi;
	for (SIZE_T addr = (SIZE_T)src; addr > (SIZE_T)src - 0x80000000; addr = (SIZE_T)mbi.BaseAddress - 1)
	{
		if (!VirtualQuery((LPCVOID)addr, &mbi, sizeof(mbi)))
		{
			break;
		}

		if (mbi.State == MEM_FREE)
		{
			if (presenthook64 = (HookContext*)VirtualAlloc(mbi.BaseAddress, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE))
			{
				break;
			}
		}
	}

	// If allocating a memory page failed, the function failed. 
	if (!presenthook64)
	{
		return NULL;
	}

	// Select a pointer slot for the memory page to be freed on unload. 
	for (int i = 0; i < sizeof(detourBuffer) / sizeof(void*); ++i)
	{
		if (!detourBuffer[i])
		{
			detourBuffer[i] = presenthook64;
			break;
		}
	}

	// Save original code and apply detour. The detour code is: 
	// push rax 
	// movabs rax, 0xCCCCCCCCCCCCCCCC 
	// xchg rax, [rsp] 
	// ret 
	BYTE detour[] = { 0x50, 0x48, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x87, 0x04, 0x24, 0xC3 };
	const unsigned int length = DisasmLengthCheck((SIZE_T)src, jumplength);
	memcpy(presenthook64->original_code, src, length);
	memcpy(&presenthook64->original_code[length], detour, sizeof(detour));
	*(SIZE_T*)&presenthook64->original_code[length + 3] = (SIZE_T)src + length;

	// Build a far jump to the destination function. 
	*(WORD*)&presenthook64->far_jmp = 0x25FF;
	*(DWORD*)(presenthook64->far_jmp + 2) = (DWORD)((SIZE_T)presenthook64 - (SIZE_T)src + FIELD_OFFSET(HookContext, dst_ptr) - 6);
	presenthook64->dst_ptr = (SIZE_T)dest;

	// Write the hook to the original function. 
	DWORD flOld = 0;
	VirtualProtect(src, 6, PAGE_EXECUTE_READWRITE, &flOld);
	memcpy(src, presenthook64->far_jmp, sizeof(presenthook64->far_jmp));
	VirtualProtect(src, 6, flOld, &flOld);

	// Return a pointer to the original code. 
	return presenthook64->original_code;
}

const unsigned int DisasmLengthCheck(const SIZE_T address, const unsigned int jumplength)
{
	DISASM disasm;
	memset(&disasm, 0, sizeof(DISASM));

	disasm.EIP = (UIntPtr)address;
	disasm.Archi = 0x40;

	unsigned int processed = 0;
	while (processed < jumplength)
	{
		const int len = Disasm(&disasm);
		if (len == UNKNOWN_OPCODE)
		{
			++disasm.EIP;
		}
		else
		{
			processed += len;
			disasm.EIP += len;
		}
	}

	return processed;
}

const void* DetourFuncVTable(SIZE_T* src, const BYTE* dest, const DWORD index)
{
	DWORD dwVirtualProtectBackup;
	SIZE_T* const indexPtr = &src[index];
	const void* origFunc = (void*)*indexPtr;
	VirtualProtect(indexPtr, sizeof(SIZE_T), PAGE_EXECUTE_READWRITE, &dwVirtualProtectBackup);
	*indexPtr = (SIZE_T)dest;
	VirtualProtect(indexPtr, sizeof(SIZE_T), dwVirtualProtectBackup, &dwVirtualProtectBackup);
	return origFunc;
}
