#include "studiopch.h"
#include "Window.h"

#include <tchar.h>
#include <backends/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace AmpStudio {
	namespace Application {

        LRESULT WINAPI WndProc(HWND m_HWND, UINT msg, WPARAM wParam, LPARAM lParam);

		Window::Window() : m_HWND(NULL), m_WC(), m_Renderer()
		{

		}

		Window::~Window()
		{
			DestroyWindow(m_HWND);
			UnregisterClass(m_WC.lpszClassName, m_WC.hInstance);
		}

		bool Window::Init()
		{
			// Create window
			m_WC = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui window"), NULL };
			::RegisterClassEx(&m_WC);
			m_HWND = ::CreateWindow(m_WC.lpszClassName, _T("AmpStudio"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, m_WC.hInstance, NULL);

			// Create d9 device
			m_Renderer = new Gui::Renderer(&m_HWND);
			m_Renderer->Init();

			// Show the window
			ShowWindow(m_HWND, SW_SHOWDEFAULT);
			UpdateWindow(m_HWND);

			LOG_ASSERT(OnInit(), "Could not initialise window sub routine OnInit()");

			return true;
		}

		bool Window::Dispatch()
		{
			MSG msg;
			while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
					return false;
			}
			// Update
			OnUpdate();

			m_Renderer->BeginFrame();
			// Draw
			OnDraw();
			m_Renderer->EndFrame();
			m_Renderer->Render();

			return true;
		}
		

		LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
				return true;

			switch (msg)
			{
			case WM_SIZE:
				return 0;
			case WM_SYSCOMMAND:
				if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
					return 0;
				break;
			case WM_DESTROY:
				::PostQuitMessage(0);
				return 0;
			case WM_DPICHANGED:
				const RECT* suggested_rect = (RECT*)lParam;
				::SetWindowPos(hwnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
				break;
			}
			return ::DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
}