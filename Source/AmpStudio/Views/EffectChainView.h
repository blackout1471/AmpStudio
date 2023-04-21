#pragma once
#include "Application/SubWindow.h"
#include <imgui.h>

namespace AmpStudio {
	namespace Views {
		class EffectChainView : public Application::SubWindow {
		public:
			EffectChainView() : Application::SubWindow({ "Effect chain" }) {}
			~EffectChainView() {};

		protected:

			inline virtual void OnInit() override
			{

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

				ImGui::EndChild();
				ImGui::PopStyleColor();
				ImGui::EndGroup();
			}
		};
	}
}