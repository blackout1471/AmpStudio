#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>

namespace AmpStudio {
	namespace Views {
		class AudioControlView : public Application::SubWindow {
		public:
			AudioControlView() : Application::SubWindow({ "Audio control" }), m_CurrentInputValue(0), m_CurrentOutputValue(0),
				m_CurrentDbInputValue(-60), m_CurrentDbOutputValue(-60), m_DeviceNames(), m_CurrentDeviceIndex(0), m_CurrentBufferSizeIndex(0),
				m_BufferSizes({ "16", "32", "64", "128", "256", "512" }), m_CurrentSampleRateIndex(0), m_SampleRates({ "44100", "48000" })
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

			inline virtual void OnDraw() override
			{
				ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable 
					 | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

				ImGui::BeginTable("audioTable", 3, flags);

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				DrawInputSection();

				ImGui::TableSetColumnIndex(1);
				DrawDeviceSection();

				ImGui::TableSetColumnIndex(2);
				DrawOutputSection();

				ImGui::EndTable();
			}

			void DrawDeviceSection()
			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Audio device");
				DrawComboBox(m_DeviceNames, m_CurrentDeviceIndex, "##device_names");
				DrawComboBox(m_BufferSizes, m_CurrentBufferSizeIndex, "##buffer_sizes");
				DrawComboBox(m_SampleRates, m_CurrentSampleRateIndex, "##sample_rates");
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

			void DrawComboBox(const std::vector<std::string>& items, int& index, const char* label = 0)
			{
				if (ImGui::BeginCombo(label, items[index].c_str())) {
					for (int i = 0; i < items.size(); ++i) {
						const bool isSelected = (index == i);
						if (ImGui::Selectable(items[i].c_str(), isSelected))
							index = i;

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}

		private:
			AmpProcessing::AudioEngine* m_AudioEngine;

			int m_CurrentDeviceIndex;
			std::vector<std::string> m_DeviceNames;

			int m_CurrentBufferSizeIndex;
			std::vector<std::string> m_BufferSizes;

			int m_CurrentSampleRateIndex;
			std::vector<std::string> m_SampleRates;

			int m_CurrentInputValue;
			float m_CurrentDbInputValue;

			int m_CurrentOutputValue;
			float m_CurrentDbOutputValue;
		};
	}
}