#ifndef _RPGPP_SCREENS_PROJECT_SCREEN_H
#define _RPGPP_SCREENS_PROJECT_SCREEN_H

#include "TGUI/Widgets/HorizontalWrap.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "editor.hpp"
#include "guiScreen.hpp"

namespace screens {
class ProjectScreen : public UIScreen {
  private:
	tgui::HorizontalWrap::Ptr createToolBar();
	tgui::Group::Ptr createResourcesList();
  public:
	void initItems(tgui::Group::Ptr layout) override;
	const std::string getNameOfScreen() override { return "Project"; }
};
} // namespace screens
#endif