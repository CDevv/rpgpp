#ifndef _RGPP_EDITOR_H
#define _RGPP_EDITOR_H

#include "editorGuiService.hpp"
#include "fileSystemService.hpp"
#include "project.hpp"
#include "raylib.h"
#include "translationService.hpp"
#include <memory>
#include <string>

#define RPGPP_VERSION 0.1

class Editor {
  private:
	// the current editor gui service, responsible for managing the gui.
	std::unique_ptr<EditorGuiService> guiService;
	// the translation service responsible for all the i18n.
	std::unique_ptr<TranslationService> translationService;
	std::unique_ptr<FileSystemService> fileSystem;

  public:
	Editor();
	~Editor() = default;
	// The opened project
	std::unique_ptr<Project> project;
	// The RPG++ editor's current icon image.
	Image appIcon{};
	// the current editor instance.
	static Editor *instance;
	EditorGuiService &getGui();
	TranslationService &getTranslations();
	FileSystemService &getFs();
	Project *getProject();
	void setProject(const std::string &path);
	// this sets the icon of the editor.
	static void setAppIcon(const std::string &icon_path);
	// unload editor stuff.
	void unload() const;
};

#endif