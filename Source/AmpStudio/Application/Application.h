#pragma once
#include "Window.h"
#include "SubWindow.h"
#include <vector>

namespace AmpStudio {
	namespace Application {
		class Application : public Window {
		public:
			Application();
			~Application();

			void Run();
			void Close();

		protected:
			virtual bool OnInit() override;
			virtual void OnUpdate() override;
			virtual void OnDraw() override;

		private:
			void AddWindow(SubWindow* window);

		private:
			std::vector<SubWindow*> m_SubWindows;
			static bool s_ApplicationNotCreated;
		};
	}
}