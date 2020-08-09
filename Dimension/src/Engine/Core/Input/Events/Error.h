#pragma once
#include "Event.h"
#include <string>

namespace Dimension {
	class Error : public Event {
	public:
		static EventType GetEventType() {
			return EventType::Error;
		}
		virtual EventType GetType() const override { return GetEventType(); }

		void SetCode(const int Code) {
			this->Code = Code;
		}
		void SetSubSystem(const std::string& subSystem) {
			this->subSystem = subSystem;
		}
		void SetDescription(const std::string& description) {
			this->description = description;
		}

		int GetCode() {
			return Code;
		}
		std::string GetSubSystem() {
			return subSystem;
		}
		std::string GetDescription() {
			return description;
		}
		std::string GetErrorReport() {
			return "[" + subSystem + "] [" + std::to_string(Code) + "] " + description;
		}
	private:
		int Code;
		std::string subSystem;
		std::string description;
	};
}