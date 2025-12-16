#include "edui.hpp"
#include "editor.hpp"
#include "raylib.h"
#include <cstring>

EdUi::InteractStatus EdUi::Button(Rectangle rect, const char *title)
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

    Vector2 measure = MeasureTextEx(Editor::getUi().getFont(), title, app.fontSize, 1);
    float offsetX = (rect.width - measure.x) / 2.0f;
    float offsetY = (rect.height - measure.y) / 2.0f;

    Vector2 textPos = Vector2 { rect.x + offsetX, rect.y + offsetY };
    DrawTextEx(Editor::getUi().getFont(), title, textPos, app.fontSize, 1, WHITE);

    return res;
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
