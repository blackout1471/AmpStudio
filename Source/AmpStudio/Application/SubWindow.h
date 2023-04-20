#pragma once
#include <string>
#include <vector>

namespace GuitarBlade {
	namespace Application {
		struct SubWindowProperties {
			std::string Name;
		};

		class SubWindow
		{
		public:
			SubWindow(const SubWindowProperties& properties);
			virtual ~SubWindow();

			void Init();
			void Update();
			void Draw();

		public:
			const SubWindowProperties& GetProperties() { return m_Properties; }

		protected:
			virtual void OnInit() = 0;
			virtual void OnUpdate() = 0;
			virtual void OnDraw() = 0;

		private:
			SubWindowProperties m_Properties;
		};
	}
}

