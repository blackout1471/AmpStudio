#pragma once
#include <vector>
#include "Controls/EffectParameter.h"

#define M_PI 3.14159274101257324219f

namespace AmpProcessing {
	namespace Effects {
		/// <summary>
		/// Interface for how an effect processor should behave.
		/// </summary>
		class IEffectProcessor {
		public:
			IEffectProcessor(const std::string& name) : m_Parameters(), m_Name(name), m_CanProcess(true) {};
			~IEffectProcessor() {};

			/// <summary>
			/// Callback method for when a sample has to be processed.
			/// </summary>
			/// <param name="sample">- The samples which should be processed.</param>
			virtual void Process(std::vector<float>& sample) = 0;

			/// <summary>
			/// Toggles between whether the effect processor should process or not.
			/// </summary>
			inline void ToggleCanProcess() { m_CanProcess = !m_CanProcess; }

			/// <returns>Whether the effect processor can process.</returns>
			inline const bool GetCanProcess() const { return m_CanProcess; }

			/// <returns>The name of the effect processor.</returns>
			inline const std::string& GetName() const { return m_Name; }

			/// <returns>The parameters which the effect processor is currently using.</returns>
			inline std::vector<std::shared_ptr<Controls::EffectParameter>>& GetParameters() { return m_Parameters; }
			
			/// <summary>
			/// Adds a new effect parameter that the effect processor can use.
			/// </summary>
			/// <param name="...args">- Arguments for constructing an effect parameter</param>
			/// <returns>The newly created effect parameter.</returns>
			template<typename... Args>
			inline const std::shared_ptr<Controls::EffectParameter> AddParameter(Args&&... args) {
				auto parameter = std::make_shared<Controls::EffectParameter>(std::forward<Args>(args)...);
				m_Parameters.push_back(parameter);
				return parameter;
			};

		protected:
			/// <summary>
			/// Sets whether the effect processor can process right now.
			/// </summary>
			inline void SetCanProcess(bool value) {
				m_CanProcess = value;
			};

		private:
			std::vector<std::shared_ptr<Controls::EffectParameter>> m_Parameters;
			std::string m_Name;
			bool m_CanProcess;
		};
	}
}