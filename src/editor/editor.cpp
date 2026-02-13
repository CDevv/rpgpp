#include "editor.hpp"
#include "services/editorGuiService.hpp"
#include "services/fileSystemService.hpp"
#include "services/translationService.hpp"
#include "project.hpp"
#include "raylib.h"
#include <memory>

Editor *Editor::instance;
Editor::Editor() {
	instance = this;
	// NOTE: always initialize the configuration service first.
	// Otherwise, everything gets screwed over.
	this->configurationService = std::make_unique<ConfigurationService>();
	// filesystem must be initialized second, as many services depend on it.
	this->fileSystem = std::make_unique<FileSystemService>();
	this->translationService = std::make_unique<TranslationService>();
	this->themeService = std::make_unique<ThemeService>();
	this->project = std::unique_ptr<Project>{nullptr};
	this->guiService = std::make_unique<EditorGuiService>();
}

void Editor::setAppIcon(const std::string &icon_path) {
	auto img_loader = LoadImage(icon_path.c_str());
	Editor::instance->appIcon = img_loader;
	SetWindowIcon(img_loader);
}

EditorGuiService& Editor::getGui() const {
	return *guiService;
}

TranslationService& Editor::getTranslations() const {
	return *translationService;
}

ThemeService& Editor::getThemeService() const {
	return *themeService;
}

FileSystemService& Editor::getFs() const {
	return *fileSystem;
}

Project* Editor::getProject() const {
	return project.get();
}

ConfigurationService &Editor::getConfiguration() const {
	return *configurationService;
}

void Editor::setProject(const std::string& path) {
	project = std::make_unique<Project>(path);
}

void Editor::unload() const {
	// Unload all the assets currently loaded and close the window.
	UnloadImage(this->appIcon);
	CloseWindow();
}
