#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>
#include <AudioEngine.h>
#include <imgui-knobs.h>
#include "Gui/NodeEditor.h"

namespace AmpStudio {
	namespace Views {
		class EffectChainView : public Application::SubWindow {
		public:
			EffectChainView() : Application::SubWindow({ "Effect chain" }), m_AudioEngine() {}
			~EffectChainView() 
			{
			};

		protected:

			inline virtual void OnInit() override
			{
				m_AudioEngine.Init();
			};

			inline virtual void OnUpdate() override
			{
			};

			inline virtual void OnDraw() override
			{
				editor.Begin();

				auto& processors = m_AudioEngine.GetEffectsProcessors();
				for (size_t i = 0; i < processors.size(); i++)
				{
					auto* current_processor = processors[i].get();
					auto& parameters = current_processor->GetParameters();

					editor.BeginNode(current_processor);
					auto max_boundaries_x = editor.GetCurrentNode()->Size.x;
					for (size_t k = 0; k < parameters.size(); k++)
					{
						auto* currentParameter = parameters[k].get();

						auto parameter_pos_x = 40.f * (k + 1); // Calculate in local space
						if (parameter_pos_x < max_boundaries_x && k != 0)
							ImGui::SameLine();

						ImGui::PushID((currentParameter->Name + "##" + std::to_string(i)).c_str());
						editor.Parameter(currentParameter->Name, &currentParameter->Value, currentParameter->Min, currentParameter->Max);
						ImGui::PopID();
						
						
					}
					

					ImGui::NewLine();
					if (current_processor->GetCanProcess())
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 255, 0, 255));
					else
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(255, 0, 0, 255));

					if (ImGui::Button(("Toggle##" + std::to_string(i)).c_str(), { 40, 20 }))
						current_processor->ToggleCanProcess();

					ImGui::PopStyleColor();

					editor.EndNode();
				}

				editor.End();
			}
		private:
			AmpProcessing::AudioEngine m_AudioEngine;
			Gui::NodeEditor editor;
		};
	}
}