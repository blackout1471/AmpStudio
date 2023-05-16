#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>
#include "Singleton.h"
#include <Effects/EffectCategory.h>

namespace AmpStudio {
	namespace Views {
		class AudioEffectsView : public Application::SubWindow {
		public:
			AudioEffectsView() : Application::SubWindow({ "Audio Effects" }), 
				m_Categories(AmpProcessing::Effects::ToStringVector(AmpProcessing::Effects::EffectCategory::Cabinet)) {}
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


				for (size_t i = 0; i < m_Categories.size(); i++)
				{
					if (!ImGui::CollapsingHeader(m_Categories[i].c_str()))
						continue;
				
					ImGui::Columns(2, nullptr, false);

					int columnCounter = 0;

					for (size_t k = 0; k < effects.size(); k++)
					{
						const int category = static_cast<int>(effects[k]->GetCategory());
						if (category != i)
							continue;

						if (columnCounter != 0 && columnCounter % 2 == 0)
							ImGui::NextColumn();

						auto& name = effects[k]->GetName();
						auto label = name + "##" + std::to_string(k);
						if (ImGui::Button(label.c_str(), { 125, 25 }))
							Singleton::getInstance().GetAudio()->AddEffectToChain(name);

						columnCounter++;
					}

					ImGui::Columns();

				}
			}

		private:
			std::vector<std::string> m_Categories;
		};
	}
}