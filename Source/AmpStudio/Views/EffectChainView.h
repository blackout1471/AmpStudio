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
					for (size_t i = 0; i < parameters.size(); i++)
					{
						auto* currentParameter = parameters[i].get();

						DrawParameter(currentParameter);
						ImGui::SameLine();
					}

					editor.EndNode();
				}

				editor.End();

				/*ImGuiIO& io = ImGui::GetIO();
				auto* drawList = ImGui::GetWindowDrawList();
				auto p = ImGui::GetCursorScreenPos();


				ImGui::BeginGroup();
				ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(60, 60, 70, 200));
				ImGui::BeginChild("scroll_view", { 0, 0 }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
				drawList->AddRectFilled(p, { p.x + 40, p.y + 40 }, IM_COL32(255, 0, 0, 255), 0.5f);

				auto& processors = m_AudioEngine.GetEffectsProcessors();
				for (size_t i = 0; i < processors.size(); i++)
				{
					ImGui::BeginGroup();

					auto& processor = processors[i];

					ImGui::Text(processor->GetName().c_str());

					if (ImGui::Button(("Toggle Effect##" + std::to_string(i)).c_str(), { 50, 20 }))
						processor->ToggleCanProcess();

					ImGui::BeginGroup();

					auto& controls = processor->GetParameters();
					for (size_t k = 0; k < controls.size(); k++)
					{

						auto control = controls[k].get();
						DrawParameter(control);
						ImGui::SameLine();

					}

					ImGui::EndGroup();
					ImGui::EndGroup();
				}

				ImGui::EndChild();
				ImGui::PopStyleColor();
				ImGui::EndGroup();*/
			}

			inline void DrawParameter(AmpProcessing::Controls::EffectParameter* control) {
				auto step = (control->Max - control->Min) * 0.001f;
				ImGuiKnobs::Knob((control->Name).c_str(), &control->Value, control->Min, control->Max, step, "%.2f", ImGuiKnobVariant_Tick);
			}

			inline void DrawProcessorEffect(const AmpProcessing::Effects::IEffectProcessor* effect) {
				
			}
		private:
			AmpProcessing::AudioEngine m_AudioEngine;
		};
	}
}