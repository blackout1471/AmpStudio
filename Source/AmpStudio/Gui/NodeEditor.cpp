#include "studiopch.h"
#include "NodeEditor.h"
#include <imgui-knobs.h>

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

		void NodeEditor::BeginNode(AmpProcessing::Effects::IEffectProcessor* effect)
		{
			auto node = InsertNewNode(effect->GetName(), m_Settings.NodeDefaultSize);

			// Draw box
			ImVec2 pMin = m_Context.ScreenPos + node->Position;
			ImVec2 pMax = pMin + node->Size;
			m_Context.DrawList->AddRectFilled(pMin, pMax, m_Settings.NodeBackgroundColor, 5.f);
			m_Context.DrawList->AddRectFilled(pMin, { pMax.x, pMin.y + m_Settings.NodeTitleHeight }, m_Settings.NodeTitleBackgroundColor, 5.f);

			// Draw text
			auto* text = effect->GetName().c_str();
			auto titleSize = ImGui::CalcTextSize(text);
			auto ytCenter = (m_Settings.NodeTitleHeight / 2) - (titleSize.y / 2);
			auto xtCenter = (node->Size.x / 2) - (titleSize.x / 2);
			m_Context.DrawList->AddText(pMin + ImVec2{ xtCenter, ytCenter }, m_Settings.NodeTitleForegroundColor, text);

			auto start_pos = pMin + ImVec2(0.f, m_Settings.NodeTitleHeight);
			ImGui::SetCursorScreenPos(start_pos);
			ImGui::BeginGroup();
		}

		void NodeEditor::EndNode()
		{
			ImGui::EndGroup();
			ImGui::SetCursorPos(m_Context.ScreenPos);
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
			ImGui::BeginChild("Node_editor", { 0, 0 }, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse);
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
		std::vector<Node>::iterator NodeEditor::InsertNewNode(const std::string& name, const ImVec2 size)
		{
			auto node = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&](const Node& node) {return node.Name == name; });

			if (node == m_Nodes.end()) {
				if (m_Nodes.size() != 0)
					m_Nodes.push_back({ name, m_Nodes[m_Nodes.size() - 1].Position + ImVec2(size.x + m_Settings.NodeGap, 0), size });
				else
				{
					m_Nodes.push_back({ name, {20.f, 20.f}, size });
				}
				node = std::prev(m_Nodes.end());
			}

			return node;
		}
	}
}