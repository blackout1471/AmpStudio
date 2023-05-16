#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>

namespace AmpStudio {
	namespace Views {
		class AudioControlView : public Application::SubWindow {
		public:
			AudioControlView() : Application::SubWindow({ "Audio control" }), m_CurrentInputValue(0), m_CurrentOutputValue(0),
				m_CurrentDbInputValue(-60), m_CurrentDbOutputValue(-60), m_DeviceNames(), m_CurrentDeviceIndex(0), m_CurrentBufferSizeIndex(0),
				m_BufferSizes({ "16", "32", "64", "128", "256", "512"}), m_CurrentSampleRateIndex(0), m_SampleRates({ "44100", "48000"}),
				m_DebugDevice(false)
			{}
			~AudioControlView() {};

		protected:
			inline static bool StringGetter(void* data, int index, const char** out_str) {

				*out_str = (*static_cast<std::vector<std::string>*>(data))[index].c_str();

				return true;
			}

			inline static int32_t GetIndex(const std::vector<std::string>& vec, int value) {
				auto str = std::to_string(value);

				auto iter = std::find(vec.begin(), vec.end(), str);
				if (iter != vec.end())
					return std::distance(vec.begin(), iter);

				return -1;
			};

			inline virtual void OnInit() override
			{
				m_AudioEngine = Singleton::getInstance().GetAudio();
				m_DeviceNames = m_AudioEngine->GetAvailableDevices();

				const auto& details = m_AudioEngine->GetDeviceDetails();

				m_CurrentSampleRateIndex = GetIndex(m_SampleRates, (int)details.sampleRate);
				m_CurrentBufferSizeIndex = GetIndex(m_BufferSizes, details.prefferedBufferSize);
			};


			inline virtual void OnUpdate() override
			{
				m_DeltaTime += ImGui::GetIO().DeltaTime;

				if (m_DeltaTime < 0.03f) return;

				m_DeltaTime = 0.f;

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

				// TODO: Refactor this.
				ImGui::SameLine();
				if (ImGui::Checkbox("Debug device", &m_DebugDevice))
				{
					if (m_DebugDevice)
						m_AudioEngine->SetDebugDevice();
					else
						m_AudioEngine->SetNewDevice("");

					m_DeviceNames = m_AudioEngine->GetAvailableDevices();

					const auto& details = m_AudioEngine->GetDeviceDetails();
				}

				ImGui::PushItemWidth(-FLT_MIN);

				if (ImGui::Combo("##device_names", &m_CurrentDeviceIndex, &StringGetter, &m_DeviceNames, m_DeviceNames.size()))
					m_AudioEngine->SetNewDevice(m_DeviceNames[m_CurrentDeviceIndex]);
				
				
				if (ImGui::Combo("##buffer_sizes", &m_CurrentBufferSizeIndex, &StringGetter, &m_BufferSizes, m_BufferSizes.size()))
					m_AudioEngine->SetBufferSize(std::stoul(m_BufferSizes[m_CurrentBufferSizeIndex]));

				if (ImGui::Combo("##sample_rates", &m_CurrentSampleRateIndex, &StringGetter, &m_SampleRates, m_SampleRates.size()))
					m_AudioEngine->SetSampleRate(std::stoul(m_SampleRates[m_CurrentSampleRateIndex]));

				ImGui::PopItemWidth();
			}


			void DrawInputSection()
			{
				auto* engine = m_AudioEngine;

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Input volume");
				ImGui::PushItemWidth(-FLT_MIN);
				ImGui::ProgressBar(m_CurrentDbInputValue, {-FLT_MIN, 0}, "");
				if (ImGui::SliderInt("##input_slider", &m_CurrentInputValue, -60, 6, "%d db"))
					engine->SetDesiredInputDbLevel(m_CurrentInputValue);

				ImGui::PopItemWidth();
			}

			void DrawOutputSection()
			{
				auto* engine = m_AudioEngine;

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Output volume");
				ImGui::PushItemWidth(-FLT_MIN);
				ImGui::ProgressBar(m_CurrentDbOutputValue, { -FLT_MIN, 0 }, "");
				if (ImGui::SliderInt("##output_slider", &m_CurrentOutputValue, -60, 6, "%d db"))
					engine->SetDesiredOutputDbLevel(m_CurrentOutputValue);

				ImGui::PopItemWidth();
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

			bool m_DebugDevice;

			float m_DeltaTime = 0.f;
		};
	}
}