#include "editor.hpp"
#include "editorGuiService.hpp"
#include "project.hpp"
#include "raylib.h"
#include "translationService.hpp"
#include <memory>

Editor *Editor::instance;
Editor::Editor() {
	Editor::instance = this;
	this->project = std::unique_ptr<Project>{nullptr};
	this->guiService = std::make_unique<EditorGuiService>();
	this->translationService = std::make_unique<TranslationService>();
}

void Editor::setAppIcon(const std::string &icon_path) {
	auto img_loader = LoadImage(icon_path.c_str());
	Editor::instance->appIcon = img_loader;
	SetWindowIcon(img_loader);
}

EditorGuiService& Editor::getGui() {
	return *guiService;
}

TranslationService& Editor::getTranslations() {
	return *translationService;
}

Project* Editor::getProject() {
	return project.get();
}

void Editor::setProject(const std::string& path) {
	project = std::make_unique<Project>(path);
}

void Editor::unload() const {
	// Unload all the assets currently loaded and close the window.
	UnloadImage(this->appIcon);
	CloseWindow();
}