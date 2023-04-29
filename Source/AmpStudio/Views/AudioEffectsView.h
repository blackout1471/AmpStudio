#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>
#include <Systems/FileWatcherSystem.h>

static AmpProcessing::Systems::FileWatcherSystem s_System =
		AmpProcessing::Systems::FileWatcherSystem("C:\\Repos\\resources\\");

namespace AmpStudio {
	namespace Views {
		class AudioEffectsView : public Application::SubWindow {
		public:
			AudioEffectsView() : Application::SubWindow({ "Audio Effects" }) {}
			~AudioEffectsView() {};

		protected:

			inline virtual void OnInit() override
			{
				s_System.GetNewFilesInDirectory();
			};

			inline virtual void OnUpdate() override
			{
			};

			inline virtual void OnDraw() override
			{
				DrawDistortionMenu();

				DrawReverbsMenu();

				DrawAmpsMenu();

				DrawCabinetsMenu();
			}

			inline void DrawDistortionMenu()
			{
				if (!ImGui::CollapsingHeader("Distortions"))
					return;
			};

			inline void DrawReverbsMenu()
			{
				if (!ImGui::CollapsingHeader("Reverbs"))
					return;
			};

			inline void DrawAmpsMenu()
			{
				if (!ImGui::CollapsingHeader("Amps"))
					return;
			};

			inline void DrawCabinetsMenu()
			{
				if (!ImGui::CollapsingHeader("Cabinets"))
					return;
			};
		};
	}
}