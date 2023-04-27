#pragma once
#include <imgui.h>
#include <Effects/IEffectProcessor.h>

static const ImVec2& operator+(const ImVec2& v1, const ImVec2& v2) {
	return ImVec2{ v1.x + v2.x, v1.y + v2.y };
}

namespace AmpStudio {
	namespace Gui {

		struct NodeContext {
			ImVec2 ScreenPos;
			ImVec2 ScreenSize;
			ImDrawList* DrawList;
		};

		struct NodeEditorSettings {
			uint32_t BackgroundColor = IM_COL32(30, 30, 30, 255);
			uint32_t GridColor = IM_COL32(255, 255, 255, 60);
			uint32_t NodeBackgroundColor = IM_COL32(128, 128, 128, 255);
			uint32_t NodeTitleBackgroundColor = IM_COL32(200, 30, 30, 255);
			uint32_t NodeTitleForegroundColor = IM_COL32(255, 255, 255, 255);
			
			ImVec2 NodeDefaultSize = ImVec2(200, 300);
			
			float NodeTitleHeight = 35;
			float NodeGap = 50;
		};

		struct Node {
			std::string Name;
			ImVec2 Position;
			ImVec2 Size;
		};

		class NodeEditor {
		public:
			NodeEditor() : m_Settings(), m_Context(), m_Nodes(), m_CurrentNode(nullptr) {};
			~NodeEditor() {};

			void Begin();
			void End();

			void BeginNode(AmpProcessing::Effects::IEffectProcessor* effect);
			void EndNode();

			void Parameter(const std::string& name, float* value, float min, float max);

			const Node* GetCurrentNode() const { return m_CurrentNode; }

		private:
			void SetContext();
			void DrawBackground();
			void DrawGrids();

			std::vector<Node>::iterator InsertNewNode(const std::string& name, const ImVec2 size);

		private:
			NodeContext m_Context;
			const NodeEditorSettings m_Settings;
			std::vector<Node> m_Nodes;
			Node* m_CurrentNode;
		};
	}
}