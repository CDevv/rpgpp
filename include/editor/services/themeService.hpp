#include "TGUI/Loading/Theme.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;

constexpr const char *THEME_DIR = "themes";
constexpr const char *DEFAULT_THEME = "Dark";

class ThemeService {
  public:
	ThemeService();
	std::string current_theme_name = DEFAULT_THEME;
	std::shared_ptr<tgui::Theme> current_theme;
	void setTheme(const string &themeName);
	std::vector<std::string> getThemes();
	std::map<std::string, std::string> themes = {};
};
