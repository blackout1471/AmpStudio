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

				ImGui::BeginTable("table1", 2, flags);

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				DrawInputSection();

				ImGui::TableSetColumnIndex(1);
				DrawOutputSection();

				ImGui::EndTable();
			}

			void DrawInputSection()
			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Input volume");
				ImGui::PushItemWidth(-FLT_MIN);
				ImGui::ProgressBar(m_CurrentDbInputValue, {-FLT_MIN, 0}, "");
				ImGui::SliderInt("##input_slider", &m_CurrentInputValue, -60, 6);
				ImGui::PopItemWidth();
			}

			void DrawOutputSection()
			{
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Output volume");
				ImGui::PushItemWidth(-FLT_MIN);
				ImGui::ProgressBar(m_CurrentDbOutputValue, {-FLT_MIN, 0}, "");
				ImGui::SliderInt("##output_sluder", &m_CurrentOutputValue, -60, 6);
				ImGui::PopItemWidth();
			}

		private:
			int m_CurrentInputValue;
			float m_CurrentDbInputValue;

			int m_CurrentOutputValue;
			float m_CurrentDbOutputValue;
		};
	}
}