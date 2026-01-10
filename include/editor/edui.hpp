#ifndef _RPGPP_EDITOR_EDUI_H
#define _RPGPP_EDITOR_EDUI_H

#include <raylib.h>

#define EDUI_CURSOR_DEBOUNCE (5)

namespace EdUi {
    struct Appearance {
        Color primaryColor;
        Color secondaryColor;
        Color border;
        float fontSize;
        Color textColor;
    };

    struct UiState {
        int currentId = 0;
        Vector2 mousePos = Vector2 { 0, 0 };
        int mouseDown = 0;
        int hotItem = 0;
        int activeItem = 0;
        int kbdItem = 0;
        int keyEntered = 0;
        int keyDown = 0;
        int keyChar = 0;
        int keyMod = 0;
        int lastWidget = 0;
        int cursorIndex = -1;
        Vector2 drawOffset = Vector2 { 0, 0 };
        int cursorDebounce = EDUI_CURSOR_DEBOUNCE;
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

    void End();
    bool Button(int id, Rectangle rect, const char* title);
    int TextField(int id, Rectangle rect, char* buffer, int size);
    InteractStatus IconButton(Rectangle rect, Texture texture);
    void BeginAppMenu();
    void EndAppMenu();
    InteractStatus MenuButton(const char* title);
    void BeginAppSubmenu(const char* title);
    void EndAppSubmenu();
    InteractStatus SubmenuButton(const char* title);
}

#endif
