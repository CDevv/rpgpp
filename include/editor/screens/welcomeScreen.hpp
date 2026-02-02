#ifndef _RPGPP_WELCOME_SCREEN_H
#define _RPGPP_WELCOME_SCREEN_H

#include "TGUI/Widgets/Group.hpp"
#include "guiScreen.hpp"
#include "newFileDialog.hpp"

namespace screens {
class WelcomeScreen : public UIScreen {
  private:
	NewFileDialog::Ptr newProjectDialog;

  public:
	void initItems(tgui::Group::Ptr layout) override;
	const std::string getNameOfScreen() override { return "Welcome"; }
};
} // namespace screens
#endif