#include "propPropertiesBox.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "imgui.h"
#include "interactable.hpp"
#include <raylib.h>
#include <string>

PropPropertiesBox::PropPropertiesBox()
{
    prop = nullptr;
    rectPos = { 0, 0 };
    rectSize = { 0, 0 };
    collisionViewActive = false;
    propHasInteractable = false;
    intType = INT_BLANK;
}

void PropPropertiesBox::setDefaults()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.isAvailable(FILE_PROP)) {
        prop = fs.getCurrentFile()->getData<Prop>();

        Rectangle atlasRect = prop->getAtlasRect();
        rectPos = { static_cast<int>(atlasRect.x), static_cast<int>(atlasRect.y) };
        rectSize = { static_cast<int>(atlasRect.width), static_cast<int>(atlasRect.height) };
        propHasInteractable = prop->getHasInteractable();
    }
}

void PropPropertiesBox::update()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.isAvailable(FILE_PROP)) {
        this->prop = fs.getCurrentFile()->getData<Prop>();

        Rectangle newAtlasRect = Rectangle {
            static_cast<float>(rectPos[0]),
            static_cast<float>(rectPos[1]),
            static_cast<float>(rectSize[0]),
            static_cast<float>(rectSize[1])
        };
        prop->setAtlasRect(newAtlasRect);
        if (propHasInteractable) {
            prop->setInteractableType(static_cast<InteractableType>(intType));
        }
    }
}

void PropPropertiesBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (prop == nullptr) return;

    ImGui::Checkbox("Collision View", &collisionViewActive);

    std::string imageSource = std::string(prop->getImagePath());

    ImGui::InputText("Image", imageSource.data(), imageSource.size(),
        ImGuiInputTextFlags_ReadOnly);

    if (ImGui::Button("Change Image..", ImVec2(-1, 0))) {
        FS_Result fsResult = fs.openImage();
        if (fsResult.result == NFD_OKAY) {
            prop->setTextureFromPath(fsResult.path);
        }
    }

    ImGui::InputInt2("Atlas X, Y", rectPos.data());
    ImGui::InputInt2("Atlas Width, Height", rectSize.data());

    ImGui::Checkbox("Has Interactable?", &propHasInteractable);

    std::string intTypeNames = "";
    for (auto typeNameStr : Interactable::getTypeNames()) {
        intTypeNames = intTypeNames.append(typeNameStr);
        intTypeNames.push_back('\0');
    }

    if (propHasInteractable) {
        ImGui::Combo("Interactable Type", &intType, intTypeNames.c_str(), INTTYPE_MAX);
    }
}

bool PropPropertiesBox::getCollisionViewActive()
{
    return collisionViewActive;
}
