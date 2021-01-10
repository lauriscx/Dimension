#pragma once
#include <vector>
#include <map>
#include <functional>
#include <tuple>
#include <iostream>
#include <future>

class Resources {
public:																		
	enum class Type {
		Text,
		Texture,
		Object,
		Sound,
		UserCustom1,
		UserCustom2,
		Count
	};

	enum class Load {
		Sync,
		Async,
		Count
	};
public:
	Resources();

	void AttachFunctionToResourceType(Type type, std::function<void(std::string, std::map<std::string, void*>*)> function);

	void LoadResource(Type type, Load load, std::string path);
	void* GetResource(const char* path);

	~Resources();

private:
	std::map<std::string, void*> _Resources;

	std::map<Type, std::function<void(std::string, std::map<std::string, void*>*)>> functions;
	std::map<std::string, std::future<void>> _Procesing;
};