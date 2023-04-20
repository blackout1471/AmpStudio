#pragma once
#include <string>
#include "Renderer.h"

#include <Windows.h>

namespace GuitarBlade {
	namespace Application {
		class Window {
		public:
			Window();
			virtual ~Window();

		protected:
			bool Init();
			bool Dispatch();

		protected:
			virtual bool OnInit() = 0;
			virtual void OnUpdate() = 0;
			virtual void OnDraw() = 0;

		protected:
			const Gui::Renderer* GetRenderer() { return m_Renderer; }

		private:
			WNDCLASSEX m_WC;
			HWND m_HWND;

			Gui::Renderer* m_Renderer;
		};
	}
}