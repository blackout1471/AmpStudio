#include "SubWindow.h"

namespace GuitarBlade {
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