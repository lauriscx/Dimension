#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Dimension {
	std::shared_ptr<spdlog::logger> Logger::logger;

	void Logger::Init() {
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		//logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Hazel.log", true));

		logSinks[0]->set_pattern("%^[%T] [%n]: %v%$");
		//logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		logger = std::make_shared<spdlog::logger>("Dimension", begin(logSinks), end(logSinks));
		spdlog::register_logger(logger);
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);
	}

	void Logger::Info	(const char * message) {
		logger->info(message);
	}
	void Logger::Warning(const char * message) {
		logger->warn(message);
	}
	void Logger::Error	(const char * message) {
		logger->error(message);
	}
	void Logger::Assert	(bool condition, const char * message) {
		if (condition) {
			logger->trace(message);
			__debugbreak();
		}
	}
}