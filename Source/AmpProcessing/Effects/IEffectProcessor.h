#pragma once
#include <vector>
#include "Controls/EffectParameter.h"

#define M_PI 3.14159274101257324219f

namespace AmpProcessing {
	namespace Effects {
		class IEffectProcessor {
		public:
			IEffectProcessor(const std::string& name) : m_Parameters(), m_Name(name), m_CanProcess(true) {};
			~IEffectProcessor() {};

			virtual void Process(std::vector<float>& sample) = 0;

			inline void ToggleCanProcess() { m_CanProcess = !m_CanProcess; }
			inline const bool GetCanProcess() const { return m_CanProcess; }

			inline const std::string& GetName() const { return m_Name; }

			inline std::vector<std::shared_ptr<Controls::EffectParameter>>& GetParameters() { return m_Parameters; }
			template<typename... Args>
			inline const std::shared_ptr<Controls::EffectParameter> AddParameter(Args&&... args) {
				auto parameter = std::make_shared<Controls::EffectParameter>(std::forward<Args>(args)...);
				m_Parameters.push_back(parameter);
				return parameter;
			}

		private:
			std::vector<std::shared_ptr<Controls::EffectParameter>> m_Parameters;
			std::string m_Name;
			bool m_CanProcess;
		};
	}
}