#include "studiopch.h"
#include "NodeEditor.h"
namespace AmpStudio {
	namespace Gui {

		void NodeEditor::Begin()
		{
			DrawBackground();
			SetContext();
			DrawGrids();
		}

		void NodeEditor::End()
		{
			ImGui::EndChild();
			ImGui::PopStyleColor();
		}

		void NodeEditor::DrawEffectNode(const AmpProcessing::Effects::IEffectProcessor* effect)
		{
			auto& node = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&](const Node& node) {return node.Name == effect->GetName(); });

			if (node == m_Nodes.end()) {
				m_Nodes.push_back({ effect->GetName(), {10, 10}, {300, 300} });
				node = std::prev(m_Nodes.end());
			}

			// Draw box
			ImVec2 pMin = m_Context.ScreenPos + node->Position;
			ImVec2 pMax = pMin + node->Size;
			m_Context.DrawList->AddRectFilled(pMin, pMax, m_Settings.NodeBackgroundColor, 5.f);
			m_Context.DrawList->AddRectFilled(pMin, { pMax.x, pMin.y + 50 }, m_Settings.NodeTitleBackgroundColor, 5.f); // 50 = height, set in settings

			// Draw text
			auto* text = effect->GetName().c_str();
			auto titleSize = ImGui::CalcTextSize(text);
			auto ytCenter = (50 / 2) - (titleSize.y / 2); // 50 = height, set in settings
			auto xtCenter = (node->Size.x / 2) - (titleSize.x / 2);
			m_Context.DrawList->AddText(pMin + ImVec2{ xtCenter, ytCenter }, IM_COL32(255, 255, 255, 255), text);

			// Draw knobs

		}

		void NodeEditor::SetContext()
		{
			m_Context =
			{
				ImGui::GetCursorScreenPos(),
				ImGui::GetWindowViewport()->Size,
				ImGui::GetWindowDrawList()
			};
		}

		void NodeEditor::DrawBackground()
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, m_Settings.BackgroundColor);
			ImGui::BeginChild("Node_editor", { 0, 0 }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
		}

		void NodeEditor::DrawGrids()
		{
			const int cellSize = 30;

			float xAmount = m_Context.ScreenSize.x / cellSize;
			float yAmount = m_Context.ScreenSize.y / cellSize;

			for (int x = 0; x <= xAmount; x++)
			{
				float xpos = m_Context.ScreenPos.x + x * cellSize - 5;
				m_Context.DrawList->AddLine(ImVec2(xpos, m_Context.ScreenPos.y), ImVec2(xpos, m_Context.ScreenPos.y + yAmount * cellSize), m_Settings.GridColor);
			}
			for (int y = 0; y <= yAmount; y++)
			{
				float ypos = m_Context.ScreenPos.y + y * cellSize - 5;
				m_Context.DrawList->AddLine(ImVec2(m_Context.ScreenPos.x, ypos), ImVec2(m_Context.ScreenPos.x + xAmount * cellSize, ypos), m_Settings.GridColor);
			}
		}
	}
}