#include "services/childWindowSubService.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "childWindows/aboutWindow.hpp"
#include "childWindows/popupWindow.hpp"
#include "childWindows/settingsWindow.hpp"
#include "editor.hpp"
#include "services/translationService.hpp"
#include <memory>

ChildWindowSubService::ChildWindowSubService() { this->createWindows(); }

void ChildWindowSubService::createWindows() {
	TranslationService &ts = Editor::instance->getTranslations();

	if (!this->childWindows.empty())
		this->childWindows.clear();

	this->childWindows.try_emplace(
		"about", std::unique_ptr<AboutWindow>(
					 new AboutWindow(ts.getKey("menu.about._label"))));

	this->childWindows.try_emplace(
		"options", std::unique_ptr<SettingsWindow>(
					   new SettingsWindow(ts.getKey("menu.options._label"))));
}

void ChildWindowSubService::openWindow(const std::string &windowName) {
	if (this->childWindows.count(windowName) == 1)
		this->childWindows[windowName]->open();
}

void ChildWindowSubService::resetAndOpen(const std::string windowName) {
	this->createWindows();
	std::cout << windowName << std::endl;
	this->openWindow(windowName);
}
