#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>

namespace AmpStudio {
	namespace Views {
		class AudioControlView : public Application::SubWindow {
		public:
			AudioControlView() : Application::SubWindow({ "Audio control" }) {}
			~AudioControlView() {};

		protected:

			inline virtual void OnInit() override
			{

			};

			inline virtual void OnUpdate() override
			{
			};

			inline virtual void OnDraw() override
			{
			}
		};
	}
}