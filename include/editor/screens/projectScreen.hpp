#ifndef _RPGPP_SCREENS_PROJECT_SCREEN_H
#define _RPGPP_SCREENS_PROJECT_SCREEN_H

#include "editor.hpp"
#include "guiScreen.hpp"

namespace screens {
class ProjectScreen : public UIScreen {

public:
	void initItems(tgui::Group::Ptr layout) override;
};
} // namespace screens
#endif