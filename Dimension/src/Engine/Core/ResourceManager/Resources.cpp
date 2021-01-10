#include "Resources.h"

Resources::Resources() {}

void Resources::AttachFunctionToResourceType(Type type, std::function<void(std::string, std::map<std::string, void*>*)> function) {
	functions[type] = function;
}

void Resources::LoadResource(Type type, Load load, std::string path) {
	if (load == Load::Sync) {
		functions[type](path, &_Resources);
	} else {//for Async
		_Procesing[path] = std::async(std::launch::async, functions[type], path, &_Resources);
	}
}

void * Resources::GetResource(const char * path) {
	if (_Resources.find(path) != _Resources.end()) {
		return _Resources[path];
	}
	return nullptr;
}

Resources::~Resources() {
	_Resources.clear();
	functions.clear();
	_Procesing.clear();
}