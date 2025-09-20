#include "layoutContainer.hpp"

void BeginLayout(ImVec2 offset) 
{
	isLayout = true;
	layoutOffset = offset;
	lastWindowSize = ImVec2 { 0, 0 };
	filledPart = 0;
}

void EndLayout()
{
	isLayout = false;
}

void SetWindowLayout(int index, float part)
{
	filledPart += part;

	float windowSizeX = static_cast<float>((GetScreenWidth() * part) - layoutOffset.x + (layoutOffset.x * index));
	if (filledPart >= 1.0f) {
		windowSizeX -= layoutOffset.x * 2;
	}
	ImVec2 newWindowSize = ImVec2 { 
        windowSizeX,
        static_cast<float>(GetScreenHeight() - (layoutOffset.y * 2))
    };

	ImGui::SetNextWindowPos(ImVec2 { layoutOffset.x + (layoutOffset.x * index) + lastWindowSize.x, layoutOffset.y });
    ImGui::SetNextWindowSize(newWindowSize);

    lastWindowSize = newWindowSize;
}