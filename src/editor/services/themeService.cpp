#include "TGUI/Loading/Theme.hpp"
#include "editor.hpp"
#include "services/fileSystemService.hpp"
#include <filesystem>
#include <memory>
using namespace std;
namespace fs = std::filesystem;

ThemeService::ThemeService() {
    for (const auto& theme : fs::directory_iterator(Editor::instance->getFs().getResourcePath(THEME_DIR))) {
        // each theme is a folder, where the theme name is the folder name itself
        // in each folder should always have a file called "theme.txt"
        if (theme.is_directory()) {
            const auto name = theme.path().filename().string();
            const auto themeTxtPath = theme.path() / "theme.txt";
            if (fs::exists(themeTxtPath)) {
                this->themes.try_emplace(name, themeTxtPath);
            }
        }
    }
    this->current_theme = std::make_shared<tgui::Theme>(this->themes[this->current_theme_name]);
    tgui::Theme::setDefault(this->current_theme);
}

void ThemeService::setTheme(const string& themeName) {
    if (this->themes.find(themeName) != this->themes.end()) {
        this->current_theme_name = themeName;
        this->current_theme = std::make_shared<tgui::Theme>(this->themes[this->current_theme_name]);
        tgui::Theme::setDefault(this->current_theme);
    }
}

std::vector<std::string> ThemeService::getThemes() {
    std::vector<std::string> themes;
    for (const auto& theme : this->themes) {
        themes.push_back(theme.first);
    }
    return themes;
}
