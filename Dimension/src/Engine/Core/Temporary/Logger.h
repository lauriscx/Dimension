#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Dimension {

	class Logger {
		public:
			static void Init();

			static void Succes	(const char* message);
			static void Info	(const char* message);
			static void Warning	(const char* message);
			static void Error	(const char* message);
			static void Assert	(bool condition, const char* message);
		private:
			static std::shared_ptr<spdlog::logger> logger;
	};
}

#define DSUCCES(...)		Dimension::Logger::Succes(__VA_ARGS__)
#define DINFO(...)		Dimension::Logger::Info(__VA_ARGS__)
#define DWARNING(...)	Dimension::Logger::Warning(__VA_ARGS__)
#define DERROR(...)		Dimension::Logger::Error(__VA_ARGS__)
//#define ASSERT(...)		Dimension::Logger::Assert(__VA_ARGS__);