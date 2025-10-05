#ifndef _RPGPP_EDITOR_LAYOUTCONTAINER_H
#define _RPGPP_EDITOR_LAYOUTCONTAINER_H

#include <imgui.h>
#include <raylib.h>

static bool isLayout;
static ImVec2 layoutOffset;
static ImVec2 lastWindowSize;
static float filledPart;

void BeginLayout(ImVec2 offset);
void EndLayout();
void SetWindowLayout(int index, float part);

#endif