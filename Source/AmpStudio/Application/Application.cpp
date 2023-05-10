#include "studiopch.h"
#include "Application.h"

// TODO:: Create header with all views
#include "Views/AudioEffectsView.h"
#include "Views/EffectChainView.h"
#include "Views/AudioControlView.h"

namespace AmpStudio {
	namespace Application {

		bool Application::s_ApplicationNotCreated = true;

		Application::Application() : Window(), m_SubWindows()
		{
			// Setup application data
			AmpProcessing::Logger::Logger::Get().Init();


			LOG_ASSERT(Application::s_ApplicationNotCreated, "Application already exists");
			Application::s_ApplicationNotCreated = true;

			// Add windows
			LOG_INFO("Adding views");
			AddWindow(new Views::AudioEffectsView());
			AddWindow(new Views::EffectChainView());
			AddWindow(new Views::AudioControlView());

			
			// Init
			AmpStudio::Singleton::getInstance().GetAudio()->Init();
			Window::Init();
		}

		Application::~Application()
		{
			for (auto* subWindow : m_SubWindows)
				delete subWindow;
		}

		bool Application::OnInit()
		{
			for (auto& subWindow : m_SubWindows)
				subWindow->Init();

			return true;
		}

		void Application::OnUpdate()
		{
			for (auto& subWindow : m_SubWindows)
				subWindow->Update();
		}

		void Application::OnDraw()
		{
			const auto* renderer = GetRenderer();
			for (auto& subWindow : m_SubWindows)
			{
				renderer->StartSubwindow(subWindow->GetProperties());
				subWindow->Draw();
				renderer->EndSubWindow();
			}
		}

		void Application::AddWindow(SubWindow* window)
		{
			m_SubWindows.push_back(window);
		}

		void Application::Run()
		{
			
			bool running = true;
			while (running)
			{
				running = Window::Dispatch();
			}
		}

		void Application::Close()
		{
		}
	}
}
