#include "edui.hpp"
#include "editor.hpp"
#include "raylib.h"
#include <cmath>
#include <cstdio>
#include <cstring>

void EdUi::End()
{
    UiState& state = Editor::getUi().getState();

    if (state.mouseDown == 0) {
        state.activeItem = 0;
    }
    else {
        if (state.activeItem == 0) {
            state.activeItem = -1;
        }
    }

    if (state.keyEntered == KEY_TAB) {
        state.kbdItem = 0;
    }
    state.keyDown = 0;
    state.keyEntered = 0;
    state.keyChar = 0;
}

bool EdUi::Button(int id, Rectangle rect, const char *title)
{
    UiState& state = Editor::getUi().getState();
    Appearance& app = Editor::getUi().getAppearance();
    InteractStatus res = EDUI_NONE;

    state.currentId++;

    // update
    if (CheckCollisionPointRec(state.mousePos, rect) && state.layer == 0) {
        res = EDUI_HOVER;
        state.hotItem = id;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            res = EDUI_CLICK;
            state.activeItem = id;
        }
    }
    if (state.kbdItem == 0) {
        state.kbdItem = id;
    }

    // draw
    if (res == EDUI_HOVER) {
        DrawRectangleRec(rect, app.secondaryColor);
    } else {
        DrawRectangleRec(rect, app.primaryColor);
    }

    if (state.kbdItem == id) {
        DrawRectangleLinesEx(rect, 1.0f, RAYWHITE);
    } else {
        DrawRectangleLinesEx(rect, 1.0f, app.border);
    }

    Vector2 measure = MeasureTextEx(Editor::getUi().getFont(), title, app.fontSize, 1);
    float offsetX = (rect.width - measure.x) / 2.0f;
    float offsetY = (rect.height - measure.y) / 2.0f;

    Vector2 textPos = Vector2 { rect.x + offsetX, rect.y + offsetY };
    DrawTextEx(Editor::getUi().getFont(), title, textPos, app.fontSize, 1, WHITE);

    if (state.kbdItem == id) {
        switch (state.keyEntered) {
            case KEY_TAB:
                state.kbdItem = 0;
                if (state.keyMod & KEY_LEFT_SHIFT) {
                    state.kbdItem = state.lastWidget;
                }
                state.keyEntered = 0;
                break;
            case KEY_ENTER:
                return true;
        }
    }

    state.lastWidget = id;

    if (state.mouseDown == 0 && state.hotItem == id && state.activeItem == id) {
        state.kbdItem = id;
        return true;
    } else {
        return false;
    }
}

int EdUi::TextField(int id, Rectangle rect, char *buffer, int size)
{
    int len = strlen(buffer);
    int changed = 0;

    UiState& state = Editor::getUi().getState();
    Appearance& app = Editor::getUi().getAppearance();
    InteractStatus res = EDUI_NONE;

    // update
    if (CheckCollisionPointRec(state.mousePos, rect) && state.layer == 0) {
        res = EDUI_HOVER;
        state.hotItem = id;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            res = EDUI_CLICK;
            state.activeItem = id;
        }
    }
    if (res == EDUI_HOVER) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
    if (state.kbdItem == 0) {
        state.kbdItem = id;
    }

    // draw base
    if (res == EDUI_HOVER) {
        DrawRectangleRec(rect, app.secondaryColor);
    } else {
        DrawRectangleRec(rect, app.primaryColor);
    }

    if (state.kbdItem == id) {
        DrawRectangleLinesEx(rect, 1.0f, RAYWHITE);
    } else {
        DrawRectangleLinesEx(rect, 1.0f, app.border);
    }

    // Vars for text drawing and cursor drawing
    Vector2 measure = MeasureTextEx(Editor::getUi().getFont(), buffer, app.fontSize, 1);
    float offsetX = 4.0f;
    float offsetY = (rect.height - measure.y) / 2.0f;

    const char* subtext;
    Vector2 subtextMeasure = Vector2 { 0, 0 };
    if (state.cursorIndex == -1) {
        subtext = "";
        subtextMeasure = Vector2 { 0, 0 };
    } else {
        subtext = TextSubtext(buffer, 0, state.cursorIndex);
        subtextMeasure = MeasureTextEx(Editor::getUi().getFont(), subtext, app.fontSize, 1);
    }

    

    Vector2 cursorPos = Vector2 { rect.x + offsetX + subtextMeasure.x, rect.y + offsetY };
    cursorPos.x -= state.drawOffset.x;
    //-

    if (state.kbdItem != id) {
        state.drawOffset.x = 0.0f;
    }

    // Does it go out of bounds?
    float textDrawSize = (measure.x + offsetX);
    float rectDrawSize = (rect.width - offsetX);

    //Checking cursor position
    if (cursorPos.x < 0.0f) {
        state.drawOffset.x -= (state.drawOffset.x - fabsf(cursorPos.x));
        cursorPos.x = offsetX;
    }
    else {
        if (cursorPos.x > rectDrawSize) {
            state.drawOffset.x = fabsf(textDrawSize - rectDrawSize);
            cursorPos.x = rectDrawSize;
        }
        else if (cursorPos.x < rectDrawSize) {
            if (state.cursorIndex == len && measure.x > rectDrawSize) {
                state.drawOffset.x = fabsf(rectDrawSize - textDrawSize);
                cursorPos.x = rectDrawSize;
            }
        }
    }

    //Use scissor mode
    BeginScissorMode(rect.x + 1, rect.y + 1, rect.width - 2, rect.height - 2);

    Vector2 textPos = Vector2 { rect.x + offsetX, rect.y + offsetY };
    if (state.kbdItem == id) {
        textPos.x -= state.drawOffset.x;
    }

    DrawTextEx(Editor::getUi().getFont(), buffer, textPos, app.fontSize, 1, WHITE);

    EndScissorMode();

    if (state.kbdItem == id) {
        if (state.cursorIndex == -1) {
            state.cursorIndex = len;
        }

        Rectangle cursorRect = Rectangle { cursorPos.x, cursorPos.y, 1, measure.y };
        DrawRectangleRec(cursorRect, RAYWHITE);
    } else {
        state.cursorIndex = -1;
        state.drawOffset = Vector2 { 0, 0 };
    }

    if (state.kbdItem == id) {
        switch (state.keyEntered) {
            case KEY_TAB:
                state.kbdItem = 0;
                if (state.keyMod & KEY_LEFT_SHIFT) {
                    state.kbdItem = state.lastWidget;
                }
                state.keyEntered = 0;
                break;
            case KEY_HOME:
                state.cursorIndex = 0;
                state.drawOffset = Vector2 { 0, 0 };
                break;
            case KEY_END:
                state.cursorIndex = len;
                break;
        }

        if (state.cursorDebounce > 0) {
            state.cursorDebounce--;
        }
        else {
            state.cursorDebounce = EDUI_CURSOR_DEBOUNCE;
            switch (state.keyDown) {
                case KEY_BACKSPACE:
                    if (len > 0) {
                        for (int i = state.cursorIndex - 1; i < len; i++) {
                            buffer[i] = buffer[i + 1];
                        }
                        state.cursorIndex--;
                        len--;
                        buffer[len] = 0;
                        changed = 1;
                    }
                    break;
                case KEY_LEFT:
                    if (state.cursorIndex > 0) {
                        state.cursorIndex--;
                    }
                    break;
                case KEY_RIGHT:
                    if (state.cursorIndex < len) {
                        state.cursorIndex++;
                    }
                    break;
            }
        }

        //handle key combos/shortcuts here
        if (IsKeyPressed(KEY_V) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))) {
            printf("pasting..\n");
            const char* clipboard = GetClipboardText();
            int clipLen = strlen(clipboard);

            if (clipLen + len < size) {
                for (int i = 1; i < clipLen; i++) {
                    buffer[len] = 'A';
                    len++;
                    buffer[len] = 0;
                }
                for (int i = state.cursorIndex; i < len; i++) {
                    buffer[i + clipLen] = buffer[i];
                }

                for (int i = 0; i < clipLen; i++) {
                    buffer[state.cursorIndex] = clipboard[i];
                    state.cursorIndex++;
                    len++;
                    buffer[len] = 0;
                    changed = 1;
                }
            }
        }

        // Char insertion
        if (state.keyChar >= 32 && state.keyChar <= 127 && len < size) {
            for (int i = state.cursorIndex; i < len; i++) {
                buffer[i + 1] = buffer[i];
            }
            buffer[state.cursorIndex] = state.keyChar;
            state.cursorIndex++;
            len++;
            buffer[len] = 0;
            changed = 1;
        }
    }

    if (state.mouseDown == 0 && state.hotItem == id && state.activeItem == id) {
        state.kbdItem = id;

        Vector2 mousePosLocal = Vector2 { state.mousePos.x - rect.x, state.mousePos.y - rect.y };
        int selectedIndex = len;
        for (int i = 0; i < len; i++) {
            const char* charSubText = TextSubtext(buffer, 0, i);
            Vector2 charMeasure = MeasureTextEx(Editor::getUi().getFont(), charSubText, app.fontSize, 1);
            charMeasure.x -= state.drawOffset.x;

            printf("%f\n", charMeasure.x);
            printf("%f\n", mousePosLocal.x);

            if (mousePosLocal.x < charMeasure.x) {
                if (i == 0) {
                    selectedIndex = 0;
                } else {
                    selectedIndex = i - 1;
                }
                break;
            }
        }

        state.cursorIndex = selectedIndex;
    }

    state.lastWidget = id;

    return changed;
}

EdUi::InteractStatus EdUi::IconButton(Rectangle rect, Texture texture)
{
    UiState& state = Editor::getUi().getState();
    Appearance& app = Editor::getUi().getAppearance();
    InteractStatus res = EDUI_NONE;

    // update
    if (CheckCollisionPointRec(GetMousePosition(), rect) && state.layer == 0) {
        res = EDUI_HOVER;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            res = EDUI_CLICK;
        }
    }

    // draw
    if (res == EDUI_HOVER) {
        DrawRectangleRec(rect, app.secondaryColor);
    } else {
        DrawRectangleRec(rect, app.primaryColor);
    }
    DrawRectangleLinesEx(rect, 1.0f, app.border);

    Rectangle src = Rectangle { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) };
    DrawTexturePro(texture, src, rect, Vector2 { 0, 0 }, 0.0f, WHITE);

    return res;
}

void EdUi::BeginAppMenu()
{
    Appearance& app = Editor::getUi().getAppearance();

    Rectangle rect = Rectangle { 0, 0, static_cast<float>(GetScreenWidth()), 32.0f };
    DrawRectangleRec(rect, app.primaryColor);
    DrawRectangleLinesEx(rect, 1.0f, app.border);

    Editor::getUi().getState().menu = true;
}

void EdUi::EndAppMenu()
{
    Editor::getUi().getState().claimedMenuWidth = 0.0f;
    Editor::getUi().getState().menu = false;
}

EdUi::InteractStatus EdUi::MenuButton(const char *title)
{
    if (!Editor::getUi().getState().menu) return EDUI_NONE;

    Appearance& app = Editor::getUi().getAppearance();
    InteractStatus res = EDUI_NONE;

    Rectangle rect;
    rect.x = Editor::getUi().getState().claimedMenuWidth; rect.y = 0;
    rect.height = 32 - 1;

    Vector2 measure = MeasureTextEx(Editor::getUi().getFont(), title, app.fontSize, 1);

    rect.width = measure.x + 8;

    // update
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        res = EDUI_HOVER;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Editor::getUi().getState().layer == 0) {
            res = EDUI_CLICK;
        }
    }
    if (res == EDUI_CLICK) {
        Editor::getUi().getState().layer = 1;
        strcpy(Editor::getUi().getState().current_menu, title);
        Editor::getUi().getState().current_offset = Editor::getUi().getState().claimedMenuWidth;
    }
    Editor::getUi().getState().claimedMenuWidth += rect.width;

    // draw
    if (res == EDUI_HOVER) {
        DrawRectangleRec(rect, app.secondaryColor);
    } else {
        DrawRectangleRec(rect, app.primaryColor);
    }

    float offsetX = (rect.width - measure.x) / 2.0f;
    float offsetY = (rect.height - measure.y) / 2.0f;

    Vector2 textPos = Vector2 { rect.x + offsetX, rect.y + offsetY };
    DrawTextEx(Editor::getUi().getFont(), title, textPos, app.fontSize, 1, WHITE);

    return res;
}

void EdUi::BeginAppSubmenu(const char* title)
{
    if (strlen(title) >= 256) {
        // not allowed..
    }
    strcpy(Editor::getUi().getState().menu_title, title);
    int cmp = strcmp(Editor::getUi().getState().menu_title, Editor::getUi().getState().current_menu);
    if (cmp == 0) {
        Editor::getUi().getState().submenu = true;
    } else {
        Editor::getUi().getState().layer = 0;
        Editor::getUi().getState().submenu = false;
    }
}

void EdUi::EndAppSubmenu()
{
    Rectangle rect = Rectangle {
        static_cast<float>(Editor::getUi().getState().current_offset), 32,
        200.0f, static_cast<float>(Editor::getUi().getState().claimedSubmenuHeight)
    };

    Appearance& app = Editor::getUi().getAppearance();
    if (Editor::getUi().getState().layer == 1) {
        DrawRectangleLinesEx(rect, 1.0f, app.border);
    }

    Editor::getUi().getState().submenu = false;
    Editor::getUi().getState().claimedSubmenuHeight = 0.0f;
}

EdUi::InteractStatus EdUi::SubmenuButton(const char *title)
{
    UiState& state = Editor::getUi().getState();
    Appearance& app = Editor::getUi().getAppearance();
    InteractStatus res = EDUI_NONE;

    if (state.layer != 1) return EDUI_NONE;

    Rectangle rect;
    rect.x = state.current_offset; rect.y = 32 + state.claimedSubmenuHeight;
    rect.width = 200.0f; rect.height = 20;

    state.claimedSubmenuHeight += 20;

    // update
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        res = EDUI_HOVER;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && state.layer == 1) {
            res = EDUI_CLICK;
        }
    }
    if (res == EDUI_CLICK) {
        state.layer = 0;
    }

    // draw
    if (res == EDUI_HOVER) {
        DrawRectangleRec(rect, app.secondaryColor);
    } else {
        DrawRectangleRec(rect, app.primaryColor);
    }

    Vector2 measure = MeasureTextEx(Editor::getUi().getFont(), title, app.fontSize, 1);

    float offsetX = (rect.width - measure.x) / 2.0f;
    float offsetY = (rect.height - measure.y) / 2.0f;

    Vector2 textPos = Vector2 { rect.x + offsetX, rect.y + offsetY };
    DrawTextEx(Editor::getUi().getFont(), title, textPos, app.fontSize, 1, WHITE);

    return res;
}
