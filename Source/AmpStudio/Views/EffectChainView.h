#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>
#include <AudioEngine.h>
#include <imgui-knobs.h>

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
				ImGuiIO& io = ImGui::GetIO();

				ImGui::BeginGroup();
				ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(60, 60, 70, 200));
				ImGui::BeginChild("scroll_view", { 0, 0 }, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);

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
				ImGui::EndGroup();
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