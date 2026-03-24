#include "services/recentProjectService.hpp"
#include "raylib.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

RecentProjectService::RecentProjectService() {
	path = GetWorkingDirectory();
	path /= RPGPP_RECENT_FILE;

	if (!std::filesystem::exists(path)) {
		std::ofstream file(path);
		file.close();
	}

	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open recent project file" << std::endl;
		return;
	}

	std::string s;
	while (std::getline(file, s)) {
		if (!std::filesystem::exists(s)) {
			continue;
		}
		recentProjects.push_back(s);
	}
	file.close();
	save();
}

void RecentProjectService::save() {

	std::ofstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open recent project file for saving"
				  << std::endl;
		return;
	}

	for (auto i = recentProjects.begin(); i != recentProjects.end(); ++i) {
		file << *i << std::endl;
	}

	file.close();
}

void RecentProjectService::enqueue(const std::string &projectPath) {
	for (auto i = recentProjects.begin(); i != recentProjects.end(); ++i) {
		if (*i == projectPath) {
			recentProjects.erase(i);
			break;
		}
	}
	recentProjects.push_front(projectPath);
	if (recentProjects.size() > limit) {
		recentProjects.pop_back();
	}
	save();
}

const std::deque<std::string> &RecentProjectService::getRecentProjects() const {
	return recentProjects;
}
