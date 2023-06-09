#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>
#include <imgui-knobs.h>
#include "Gui/NodeEditor.h"

#include "Singleton.h"


namespace AmpStudio {
	namespace Views {
		class EffectChainView : public Application::SubWindow {
		public:
			EffectChainView() : Application::SubWindow({ "Effect chain" }) {}
			~EffectChainView() {};

		protected:
			inline virtual void OnInit() override
			{
			};

			inline virtual void OnUpdate() override
			{
			};

			inline virtual void OnDraw() override
			{
				m_NodeEditor.Begin();
				const auto& settings = m_NodeEditor.GetEditorSettings();

				auto& processors = Singleton::getInstance().GetAudio()->GetCurrentEffectChain();
				for (size_t i = 0; i < processors.size(); i++)
				{
					auto* current_processor = processors[i];
					auto& parameters = current_processor->GetParameters();

					m_NodeEditor.BeginNode(current_processor);
					auto max_boundaries_x = m_NodeEditor.GetCurrentNode()->Size.x;
					for (size_t k = 0; k < parameters.size(); k++)
					{
						auto* currentParameter = parameters[k].get();

						auto parameter_pos_x = settings.ParameterDiameterSize * (k + 1); // Calculate in local space
						if (parameter_pos_x < max_boundaries_x && k != 0)
							ImGui::SameLine();

						ImGui::PushID((currentParameter->Name + "##" + std::to_string(i)).c_str());
						m_NodeEditor.Parameter(currentParameter->Name, &currentParameter->Value, currentParameter->Min, currentParameter->Max);
						ImGui::PopID();
					}
					
					ImGui::NewLine();
					if (current_processor->GetCanProcess())
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 255, 0, 255));
					else
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(255, 0, 0, 255));

					if (ImGui::Button(("Toggle##" + std::to_string(i)).c_str(), { 50, 20 }))
						current_processor->ToggleCanProcess();

					ImGui::PopStyleColor();

					m_NodeEditor.EndNode();
				}

				m_NodeEditor.End();
			}
		private:
			Gui::NodeEditor m_NodeEditor;
		};
	}
}