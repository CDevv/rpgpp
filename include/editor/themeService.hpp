#include <string>
#include <nlohmann/json.hpp>

using namespace std;

constexpr const char* THEME_DIR = "resources/themes";
constexpr const char* DEFAULT_THEME = "Dark";

class ThemeService {
public:
    ThemeService();
    void setTheme(const string& themeName);
    std::vector<std::string> getThemes();
    std::string getCurrentThemePath();
    std::map<std::string, std::string>
        themes = {};
    std::string current_theme = DEFAULT_THEME;
};
