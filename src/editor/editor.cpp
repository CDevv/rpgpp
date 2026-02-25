#include "editor.hpp"
#include "project.hpp"
#include "raylib.h"
#include "services/editorGuiService.hpp"
#include "services/fileSystemService.hpp"
#include "services/translationService.hpp"
#include <memory>

Editor *Editor::instance;

Editor::Editor()
	: configurationService(), translationService(this), themeService(this),
	  project{nullptr} {
	instance = this;
}

void Editor::setAppIcon(const std::string &icon_path) {
	auto img_loader = LoadImage(icon_path.c_str());
	Editor::instance->appIcon = img_loader;
	SetWindowIcon(img_loader);
}

EditorGuiService &Editor::getGui() { return guiService; }

TranslationService &Editor::getTranslations() { return translationService; }

ThemeService &Editor::getThemeService() { return themeService; }

FileSystemService &Editor::getFs() { return fileSystem; }

Project *Editor::getProject() const { return project.get(); }

ConfigurationService &Editor::getConfiguration() {
	return configurationService;
}

void Editor::setProject(const std::string &path) {
	project = std::make_unique<Project>(path);
}

void Editor::unload() const {
	// Unload all the assets currently loaded and close the window.
	UnloadImage(this->appIcon);
	CloseWindow();
}
