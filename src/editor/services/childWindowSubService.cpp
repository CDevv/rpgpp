#include "services/childWindowSubService.hpp"

#include <memory>

#include "childWindows/aboutWindow.hpp"
#include "childWindows/editPropWindow.hpp"
#include "childWindows/newPropWindow.hpp"
#include "childWindows/popupWindow.hpp"
#include "childWindows/projectSettingsWindow.hpp"
#include "childWindows/settingsWindow.hpp"

ChildWindowSubService::ChildWindowSubService() { this->createWindows(); }

void ChildWindowSubService::createWindows() {
	if (!this->childWindows.empty()) this->childWindows.clear();

	this->childWindows.try_emplace("about", std::make_unique<AboutWindow>());

	this->childWindows.try_emplace("options", std::make_unique<SettingsWindow>());

	this->childWindows.try_emplace("project_settings", std::make_unique<ProjectSettingsWindow>());

	this->childWindows.try_emplace("new_prop", std::make_unique<NewPropWindow>());

	this->childWindows.try_emplace("edit_prop", std::make_unique<EditPropWindow>());
}

void ChildWindowSubService::openWindow(const std::string &windowName) {
	if (this->childWindows.count(windowName) == 1) this->childWindows[windowName]->open();
}

void ChildWindowSubService::resetAndOpen(const std::string windowName) {
	this->createWindows();
	this->openWindow(windowName);
}

PopupWindow *ChildWindowSubService::getWindow(const std::string &windowName) {
	if (this->childWindows.count(windowName) == 1) {
		return this->childWindows[windowName].get();
	} else {
		return nullptr;
	}
}
