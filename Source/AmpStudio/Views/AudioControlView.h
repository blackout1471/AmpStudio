#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>

namespace AmpStudio {
	namespace Views {
		class AudioControlView : public Application::SubWindow {
		public:
			AudioControlView() : Application::SubWindow({ "Audio control" }), m_CurrentInputValue(0), m_CurrentOutputValue(0),
				m_CurrentInputDecibel(0.f), m_CurrentOutputDecibel(0)
			{}
			~AudioControlView() {};

		protected:

			inline virtual void OnInit() override
			{

			};

			inline virtual void OnUpdate() override
			{
			};

			inline virtual void OnDraw() override
			{
				ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable 
					 | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

				ImGui::BeginTable("table1", 2, flags);

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(1);
				DrawInputSection();

				ImGui::TableSetColumnIndex(0);
				DrawOutputSection();

				ImGui::EndTable();
			}

			void DrawOutputSection()
			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Output volume");
				ImGui::PushItemWidth(-FLT_MIN);
				ImGui::ProgressBar(m_CurrentInputDecibel);
				ImGui::SliderInt("##output_sluder", &m_CurrentOutputValue, -60, 6);
				ImGui::PopItemWidth();
			}

			void DrawInputSection()
			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Input volume");
				ImGui::PushItemWidth(-FLT_MIN);
				ImGui::ProgressBar(m_CurrentInputValue);
				ImGui::SliderInt("##input_slider", &m_CurrentInputValue, -60, 6);
				ImGui::PopItemWidth();
			}

		private:
			int m_CurrentInputValue;
			float m_CurrentInputDecibel;

			int m_CurrentOutputValue;
			float m_CurrentOutputDecibel;
		};
	}
}