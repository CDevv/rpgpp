#ifndef _RGPP_EDITOR_GUI_CONTAINER_H
#define _RGPP_EDITOR_GUI_CONTAINER_H

#include "TGUI/Backend/raylib.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/MenuBar.hpp"
#include "screens/guiScreen.hpp"
#include "updatable.hpp"
#include <memory>
#include <vector>
constexpr auto RPGPP_EXECUTABLE_LOGO = "resources/app-icon.png";
// TODO: Add theme switching.
constexpr auto CURRENT_TESTING_THEME = "resources/themes/Dark/theme.txt";
constexpr int ACTION_BUTTON_SIZE = 16;

class EditorGuiService {
  private:
	std::vector<std::weak_ptr<IUpdatable>> updatableWidgets;

  public:
	// gui service constructor.
	bool reset_gui_r = false;
	EditorGuiService();
	~EditorGuiService() = default;

	// I have no fucking clue what this is.
	// int setScreen(int _cpp_par_);

	std::unique_ptr<tgui::Gui> gui;
	std::unique_ptr<UIScreen> currentScreen;
	std::deque<std::unique_ptr<UIScreen>> screenHistory;

	std::weak_ptr<tgui::MenuBar> menuBar{};
	std::map<std::string, std::string> translations = {};
	void init();
	void uiLoop();
	void initMenuBar();
	void addUpdate(std::shared_ptr<IUpdatable> widget);

	void setScreen(std::unique_ptr<UIScreen> newScreen, bool forceSwitch);
	void setScreen(UIScreen *newScreen, bool forceSwitch);

	void resetUi();
	tgui::Group::Ptr uiChangePreInit(UIScreen *setToScreen);
	void setResetUi() { this->reset_gui_r = true; }

	void gotoPreviousScreen();
	void reloadUi();
	static void createLogoCenter(const tgui::GrowVerticalLayout::Ptr &layout);
	static void centerWidget(tgui::Widget::Ptr widget);
};

#endif
