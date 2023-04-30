#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>

#include "Singleton.h"

namespace AmpStudio {
	namespace Views {
		class AudioEffectsView : public Application::SubWindow {
		public:
			AudioEffectsView() : Application::SubWindow({ "Audio Effects" }) {}
			~AudioEffectsView() {};

		protected:

			inline virtual void OnInit() override
			{
			};

			inline virtual void OnUpdate() override
			{
			};

			inline virtual void OnDraw() override
			{
				auto& effects = Singleton::getInstance().GetAudio()->GetAvailableEffects();

				ImGui::Columns(2, nullptr, false);

				for (size_t i = 0; i < effects.size(); i++)
				{
					if (i != 0 && i % 2 == 0)
						ImGui::NextColumn();

					auto& name = effects[i]->GetName();
					auto label = name + "##" + std::to_string(i);
					if (ImGui::Button(label.c_str(), { 100, 25 }))
						Singleton::getInstance().GetAudio()->AddEffectToChain(name);

				}

				ImGui::Columns();

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