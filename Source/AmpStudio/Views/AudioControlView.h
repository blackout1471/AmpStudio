#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>

namespace AmpStudio {
	namespace Views {
		class AudioControlView : public Application::SubWindow {
		public:
			AudioControlView() : Application::SubWindow({ "Audio control" }), m_CurrentInputValue(0), m_CurrentOutputValue(0),
				m_CurrentDbInputValue(-60), m_CurrentDbOutputValue(-60)
			{}
			~AudioControlView() {};

		protected:

			inline virtual void OnInit() override
			{
				m_AudioEngine = Singleton::getInstance().GetAudio();
				m_DeviceNames = m_AudioEngine->GetAvailableDevices();
			};

			inline static float deltaTime = 0.f;

			inline virtual void OnUpdate() override
			{
				deltaTime += ImGui::GetIO().DeltaTime;

				if (deltaTime < 0.05f) return;

				deltaTime = 0.f;

				auto* engine = Singleton::getInstance().GetAudio();

				m_CurrentDbInputValue = (engine->GetInputDbLevel() + 60.f) / 60.f;
				m_CurrentDbOutputValue = (engine->GetOutputDbLevel() + 60.f) / 60.f;
			};

			inline static int currentIndex = 0;

			inline virtual void OnDraw() override
			{
				ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable 
					 | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

				ImGui::BeginTable("audioTable", 3, flags);

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				DrawInputSection();

				ImGui::TableSetColumnIndex(1);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Audio device");
				if (ImGui::BeginCombo("", m_DeviceNames[currentIndex].c_str())) {
					for (int i = 0; i < m_DeviceNames.size(); ++i) {
						const bool isSelected = (currentIndex == i);
						if (ImGui::Selectable(m_DeviceNames[i].c_str(), isSelected))
							currentIndex = i;

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				ImGui::TableSetColumnIndex(2);
				DrawOutputSection();

				ImGui::EndTable();
			}

			void DrawInputSection()
			{
				auto* engine = m_AudioEngine;

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Input volume");
				ImGui::PushItemWidth(-FLT_MIN);
				ImGui::ProgressBar(m_CurrentDbInputValue, {-FLT_MIN, 0}, "");
				if (ImGui::SliderInt("##input_slider", &m_CurrentInputValue, -60, 6))
					engine->SetDesiredInputDbLevel(m_CurrentInputValue);

				ImGui::PopItemWidth();
			}

			void DrawOutputSection()
			{
				auto* engine = m_AudioEngine;

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Output volume");
				ImGui::PushItemWidth(-FLT_MIN);
				ImGui::ProgressBar(m_CurrentDbOutputValue, {-FLT_MIN, 0}, "");
				if (ImGui::SliderInt("##output_slider", &m_CurrentOutputValue, -60, 6))
					engine->SetDesiredOutputDbLevel(m_CurrentOutputValue);

				ImGui::PopItemWidth();
			}

		private:
			AmpProcessing::AudioEngine* m_AudioEngine;

			std::vector<std::string> m_DeviceNames;

			int m_CurrentInputValue;
			float m_CurrentDbInputValue;

			int m_CurrentOutputValue;
			float m_CurrentDbOutputValue;
		};
	}
}