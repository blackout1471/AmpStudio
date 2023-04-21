#pragma once
#include "Application/SubWindow.h"

namespace AmpStudio {
	namespace Gui {
		class Renderer {
		public:
			Renderer(void* windowHandle);
			~Renderer();

		public:
			bool Init();

			void BeginFrame();
			void EndFrame();

			const void StartSubwindow(const Application::SubWindowProperties& properties) const;
			const void EndSubWindow() const;

			void Render();
			void CloseDevice();

			static void* GetDevice();

		private:
			void ResetDevice();

		private:
			void* m_WindowHandle;

		};
	}
}