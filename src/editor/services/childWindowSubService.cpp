#include "services/childWindowSubService.hpp"

#include <memory>

#include "childWindows/aboutWindow.hpp"
#include "childWindows/popupWindow.hpp"
#include "childWindows/projectSettingsWindow.hpp"
#include "childWindows/settingsWindow.hpp"

ChildWindowSubService::ChildWindowSubService() { this->createWindows(); }

void ChildWindowSubService::createWindows() {
	if (!this->childWindows.empty()) this->childWindows.clear();

	this->childWindows.try_emplace("about", std::unique_ptr<AboutWindow>(new AboutWindow()));

	this->childWindows.try_emplace("options", std::unique_ptr<SettingsWindow>(new SettingsWindow()));

	this->childWindows.try_emplace("project_settings", std::make_unique<ProjectSettingsWindow>());
}

void ChildWindowSubService::openWindow(const std::string &windowName) {
	if (this->childWindows.count(windowName) == 1) this->childWindows[windowName]->open();
}

void ChildWindowSubService::resetAndOpen(const std::string windowName) {
	this->createWindows();
	this->openWindow(windowName);
}
