#include "editor.hpp"
#include "fileSystemService.hpp"
#include <filesystem>
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
}

void ThemeService::setTheme(const string& themeName) {
    if (this->themes.find(themeName) != this->themes.end()) {
        this->current_theme = themeName;
    }
}

std::string ThemeService::getCurrentThemePath() {
    if (this->themes.find(this->current_theme) != this->themes.end()) {
        return this->themes[this->current_theme];
    }
    return "";
}

std::vector<std::string> ThemeService::getThemes() {
    std::vector<std::string> themes;
    for (const auto& theme : this->themes) {
        themes.push_back(theme.first);
    }
    return themes;
}
