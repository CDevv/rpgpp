#ifndef RPGPP_OPTIONSSCREEN_H
#define RPGPP_OPTIONSSCREEN_H
#include "guiScreen.hpp"
#include "TGUI/Widgets/Group.hpp"

namespace screens {
    class EditorOptionsScreen: public UIScreen {
    public:
        EditorOptionsScreen() = default;
        void initItems(tgui::Group::Ptr layout) override;
        const std::string getNameOfScreen() override {return "Options";};
    };
}

#endif //RPGPP_OPTIONSSCREEN_H