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
				Gui::NodeEditor editor;
				editor.Begin();

				auto& processors = m_AudioEngine.GetEffectsProcessors();
				for (size_t i = 0; i < processors.size(); i++)
				{
					auto* current_processor = processors[i].get();
					auto& parameters = current_processor->GetParameters();

					editor.BeginNode(current_processor);
					auto max_boundaries_x = ImGui::GetCursorScreenPos().x + editor.GetCurrentNode()->Size.x;
					for (size_t i = 0; i < parameters.size(); i++)
					{
						auto* currentParameter = parameters[i].get();

						editor.Parameter(currentParameter->Name, &currentParameter->Value, currentParameter->Min, currentParameter->Max);
						
						auto parameter_x = ImGui::GetCursorScreenPos().x;
						if (parameter_x < max_boundaries_x)
							ImGui::SameLine();
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
		};
	}
}