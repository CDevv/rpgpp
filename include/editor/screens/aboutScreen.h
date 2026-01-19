#ifndef RPGPP_ABOUTSCREEN_H
#define RPGPP_ABOUTSCREEN_H
#include "TGUI/Widgets/Group.hpp"
#include "guiScreen.hpp"

namespace screens {
class AboutScreen : public UIScreen {
  public:
	void initItems(tgui::Group::Ptr layout) override;
	void unloadScreen() override;
	const std::string getNameOfScreen() override { return "About"; }
};
} // namespace screens
#endif // RPGPP_ABOUTSCREEN_H