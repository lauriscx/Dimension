#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Dimension {

	class Logger {
		public:
			static void Init();

			static void Info	(const char* message);
			static void Warning	(const char* message);
			static void Error	(const char* message);
			static void Assert	(bool condition, const char* message);
		private:
			static std::shared_ptr<spdlog::logger> logger;
	};
}

#define INFO(...)		Dimension::Logger::Info(__VA_ARGS__)
#define WARNING(...)	Dimension::Logger::Warning(__VA_ARGS__)
//#define ERROR(...)		Dimension::Logger::Error(__VA_ARGS__)
//#define ASSERT(...)		Dimension::Logger::Assert(__VA_ARGS__);