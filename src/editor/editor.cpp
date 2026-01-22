#include "editor.hpp"
#include "editorGuiService.hpp"
#include "fileSystemService.hpp"
#include "project.hpp"
#include "raylib.h"
#include "translationService.hpp"
#include <memory>

Editor *Editor::instance;
Editor::Editor() {
	instance = this;
	// NOTE: always initialize the configuration service first.
	// Otherwise, everything gets screwed over.
	this->configurationService = std::make_unique<ConfigurationService>();
	this->project = std::unique_ptr<Project>{nullptr};
	this->guiService = std::make_unique<EditorGuiService>();
	this->translationService = std::make_unique<TranslationService>();
	this->fileSystem = std::make_unique<FileSystemService>();
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