#pragma once
#include <imgui.h>
#include <Effects/IEffectProcessor.h>

namespace AmpStudio {
	namespace Gui {
		static const ImVec2& operator+(const ImVec2& v1, const ImVec2& v2) {
			return ImVec2{ v1.x + v2.x, v1.y + v2.y };
		}

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
		};

		struct Node {
			std::string Name;
			ImVec2 Position;
			ImVec2 Size;
		};

		class NodeEditor {
		public:
			NodeEditor() : m_Settings(), m_Context(), m_Nodes() {};
			~NodeEditor() {};

			void Begin();
			void End();

			void DrawEffectNode(const AmpProcessing::Effects::IEffectProcessor* effect);

		private:
			void SetContext();
			void DrawBackground();
			void DrawGrids();

		private:
			NodeContext m_Context;
			const NodeEditorSettings m_Settings;
			std::vector<Node> m_Nodes;
		};
	}
}