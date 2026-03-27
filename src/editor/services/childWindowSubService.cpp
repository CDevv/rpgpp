#include "services/childWindowSubService.hpp"
#include "childWindows/aboutWindow.hpp"
#include "childWindows/popupWindow.hpp"
#include "childWindows/settingsWindow.hpp"
#include <memory>

ChildWindowSubService::ChildWindowSubService() { this->createWindows(); }

void ChildWindowSubService::createWindows() {
	if (!this->childWindows.empty())
		this->childWindows.clear();

	this->childWindows.try_emplace(
		"about", std::unique_ptr<AboutWindow>(new AboutWindow()));

	this->childWindows.try_emplace(
		"options", std::unique_ptr<SettingsWindow>(new SettingsWindow()));
}

void ChildWindowSubService::openWindow(const std::string &windowName) {
	if (this->childWindows.count(windowName) == 1)
		this->childWindows[windowName]->open();
}

void ChildWindowSubService::resetAndOpen(const std::string windowName) {
	this->createWindows();
	this->openWindow(windowName);
}
