#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "TGUI/Loading/Theme.hpp"

#ifndef RPGPP_THEMESERVICE_H
#define RPGPP_THEMESERVICE_H

class Editor;

constexpr const char *THEME_DIR = "themes";
constexpr const char *DEFAULT_THEME = "Dark";

class ThemeService {
public:
	ThemeService(Editor *editor_ptr);
	std::string current_theme_name = DEFAULT_THEME;
	std::shared_ptr<tgui::Theme> current_theme;
	void setTheme(const std::string &themeName);
	std::vector<std::string> getThemes();
	std::map<std::string, std::string> themes = {};
};

#endif