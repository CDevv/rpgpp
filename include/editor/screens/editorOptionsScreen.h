#ifndef RPGPP_OPTIONSSCREEN_H
#define RPGPP_OPTIONSSCREEN_H
#include "TGUI/Widgets/Group.hpp"
#include "guiScreen.hpp"

namespace screens {
class EditorOptionsScreen : public UIScreen {
  public:
	EditorOptionsScreen() = default;
	void initItems(tgui::Group::Ptr layout) override;
	const std::string getNameOfScreen() override { return "Options"; };
};
} // namespace screens

#endif // RPGPP_OPTIONSSCREEN_H