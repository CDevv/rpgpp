#ifndef _RGPP_EDITOR_GUI_CONTAINER_H
#define _RGPP_EDITOR_GUI_CONTAINER_H

#include "TGUI/Backend/raylib.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "screens/guiScreen.hpp"
#include <memory>
// the RPG++ executable logo.
constexpr const char *RPGPP_EXECUTABLE_LOGO = "resources/app-icon.png";
// the current theme file to use.
// TODO: Add theme switching.
constexpr const char *CURRENT_TESTING_THEME = "resources/themes/RPG2007.txt";
// The top margin for the menu.
constexpr int TOP_MARGIN = 35;
constexpr int ACTION_BUTTON_SIZE = 16;

class EditorGuiService {
  public:
	// gui service constructor.
	bool reset_gui_r = false;
	EditorGuiService();
	~EditorGuiService() = default;

	int setScreen(int _cpp_par_);

	std::unique_ptr<tgui::Gui> gui;
	UIScreen *prevScreen;
	std::unique_ptr<UIScreen> currentScreen;
	std::map<std::string, std::string> translations = {};
	void init();
	void uiLoop();
	void initMenuBar();

	void setScreen(std::unique_ptr<UIScreen> newScreen, bool forceSwitch);
	void setScreen(UIScreen *newScreen, bool forceSwitch);

	void resetUi();
	tgui::Group::Ptr uiChangePreInit(UIScreen *setToScreen);
	void setResetUi() { this->reset_gui_r = true; }

	void naviGoBack();
	void reloadUi();
	void createLogoCenter(const tgui::GrowVerticalLayout::Ptr& layout);
};
#endif
