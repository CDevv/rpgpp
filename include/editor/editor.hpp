#ifndef _RGPP_EDITOR_H
#define _RGPP_EDITOR_H

#include <memory>
#include <string>

#include "project.hpp"
#include "raylib.h"
#include "services/configurationService.hpp"
#include "services/editorGuiService.hpp"
#include "services/fileSystemService.hpp"
#include "services/hotkeyService.hpp"
#include "services/recentProjectService.hpp"
#include "services/themeService.hpp"
#include "services/translationService.hpp"

#define RPGPP_VERSION 0.1

class Editor {
private:
	// NOTE: always initialize the configuration service first.
	// Otherwise, everything gets screwed over.
	// NOTE: leave this order of fields,
	// because constructors would be called in declaration order!
	ConfigurationService configurationService;
	// filesystem must be initialized second, as many services depend on it.
	FileSystemService fileSystem;
	// dependant on Editor class itself \/
	// the translation service responsible for all the i18n.
	TranslationService translationService;
	// the current editor gui service, responsible for managing the gui.
	ThemeService themeService;
	EditorGuiService guiService;
	HotkeyService hotkeyService;
	RecentProjectService recentProjectService;

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
	ThemeService &getThemeService();
	FileSystemService &getFs();
	RecentProjectService &getRecentProjectService();
	ConfigurationService &getConfiguration();
	HotkeyService &getHotkeyService();
	Project *getProject() const;
	void setProject(const std::string &path);
	// this sets the icon of the editor.
	static void setAppIcon(const std::string &icon_path);
	// unload editor stuff.
	void unload() const;
};

#endif
