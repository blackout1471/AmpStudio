#pragma once
#include <string>
//#include <spdlog/spdlog.h>
//#include <spdlog/sinks/stdout_color_sinks.h>
//#include <spdlog/sinks/ansicolor_sink.h>

namespace AmpProcessing {
	namespace Logger {

		class Logger {
		public:
			static Logger& Get() {
				static Logger instance;
				return instance;
			}
		private:
			Logger() {};// : m_Logger(nullptr) {};
			~Logger() {};

		public:
			inline void Init() {
				/*auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
				sink->set_pattern("%^[%H:%M:%S][%l] %v%$");

				m_Logger = std::make_shared<spdlog::logger>("console", sink);
				spdlog::register_logger(m_Logger);

				GetLogger()->info("Logger initialised");*/
			}
			//inline const std::shared_ptr<spdlog::logger>& GetLogger() { return m_Logger; }

		private:
			//std::shared_ptr<spdlog::logger> m_Logger;
		};
	}
}

#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_ASSERT(condition, ...)	if (!(condition)) {__debugbreak();}

//#define LOG_INFO(...)		::AmpProcessing::Logger::Logger::Get().GetLogger()->info(__VA_ARGS__)
//#define LOG_WARN(...)		::AmpProcessing::Logger::Logger::Get().GetLogger()->warn(__VA_ARGS__)
//#define LOG_ERROR(...)		::AmpProcessing::Logger::Logger::Get().GetLogger()->error(__VA_ARGS__)
//#define LOG_ASSERT(condition, ...)	if (!(condition)) {LOG_ERROR(__VA_ARGS__); __debugbreak();}