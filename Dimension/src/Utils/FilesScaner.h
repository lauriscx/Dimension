#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
class FilesScaner {
public:

	static void Scan(std::string path, std::vector<std::string> * paths) {
		for (const auto & entry : fs::directory_iterator(path)) {
			if (entry.is_directory()) {
				Scan(entry.path().generic_string() + "/", paths);
			}
			else {
				paths->push_back(entry.path().generic_string());
			}
		}
	}
};
