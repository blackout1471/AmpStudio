#include "studiopch.h"
#include "SubWindow.h"

namespace AmpStudio {
	namespace Application {

		SubWindow::SubWindow(const SubWindowProperties& properties) : m_Properties(properties)
		{}

		SubWindow::~SubWindow()
		{
		}

		void SubWindow::Init()
		{
			OnInit();
		}

		void SubWindow::Update()
		{
			OnUpdate();
		}

		void SubWindow::Draw()
		{			
			OnDraw();
		}
	}
}