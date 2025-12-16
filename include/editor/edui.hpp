#ifndef _RPGPP_EDITOR_EDUI_H
#define _RPGPP_EDITOR_EDUI_H

#include <raylib.h>

namespace EdUi {
    struct Appearance {
        Color primaryColor;
        Color secondaryColor;
        Color border;
        float fontSize;
        Color textColor;
    };

    struct UiState {
        bool menu = false;
        bool submenu = false;
        char current_menu[256] = "";
        char menu_title[256] = "";
        float current_offset = 0.0f;
        float claimedMenuWidth = 0.0f;
        float claimedSubmenuHeight = 0.0f;
        int layer = 0;
        Rectangle layerRec = Rectangle { -1, -1, -1, -1 };
    };

    enum InteractStatus {
        EDUI_NONE = -1, EDUI_HOVER = 0, EDUI_CLICK = 1
    };

    InteractStatus Button(Rectangle rect, const char* title);
    InteractStatus IconButton(Rectangle rect, Texture texture);
    void BeginAppMenu();
    void EndAppMenu();
    InteractStatus MenuButton(const char* title);
    void BeginAppSubmenu(const char* title);
    void EndAppSubmenu();
    InteractStatus SubmenuButton(const char* title);
}

#endif
