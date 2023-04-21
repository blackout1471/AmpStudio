#include "studiopch.h"
#include "Renderer.h"

#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_win32.h>
#include <d3d9.h>

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

namespace AmpStudio {
	namespace Gui {

		Renderer::Renderer(void* windowHandle) : m_WindowHandle(windowHandle)
		{
		}

		Renderer::~Renderer()
		{
			ImGui_ImplDX9_Shutdown();
			CloseDevice();
		}


		void Renderer::CloseDevice()
		{
			if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
			if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
		}

		void* Renderer::GetDevice()
		{
			return &g_pd3dDevice;
		}

		void Renderer::ResetDevice()
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
			LOG_ASSERT(hr != D3DERR_INVALIDCALL, "Could not reset device");
			ImGui_ImplDX9_CreateDeviceObjects();
		}

		bool Renderer::Init()
		{
			HWND hwnd = *(HWND*)m_WindowHandle;

			if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
				return false;

			// Create the D3DDevice
			ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
			g_d3dpp.Windowed = TRUE;
			g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
			g_d3dpp.EnableAutoDepthStencil = TRUE;
			g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
			//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
			if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
				return false;

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			//io.ConfigViewportsNoAutoMerge = true;
			//io.ConfigViewportsNoTaskBarIcon = true;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			// Setup Platform/Renderer backends
			ImGui_ImplWin32_Init(hwnd);
			ImGui_ImplDX9_Init(g_pd3dDevice);

			return true;
		}
		
		void Renderer::BeginFrame()
		{
			// Start the Dear ImGui frame
			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			bool show_demo_window = true;
			bool show_another_window = true;
			
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

			// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);
		}


		void Renderer::EndFrame()
		{
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			// Rendering
			ImGui::EndFrame();
			g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
			D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
			g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
			if (g_pd3dDevice->BeginScene() >= 0)
			{
				ImGui::Render();
				ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
				g_pd3dDevice->EndScene();
			}
		}

		const void Renderer::StartSubwindow(const Application::SubWindowProperties& properties) const
		{
			if (!ImGui::Begin(properties.Name.c_str()))
				ImGui::End();
		}

		const void Renderer::EndSubWindow() const
		{
			ImGui::End();
		}

		void Renderer::Render()
		{
			ImGuiIO& io = ImGui::GetIO();

			// Update and Render additional Platform Windows
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}

			HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

			// Handle loss of D3D9 device
			if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
				ResetDevice();
		}
	}
}