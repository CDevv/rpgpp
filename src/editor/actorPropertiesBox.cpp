#include "actorPropertiesBox.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"

ActorPropertiesBox::ActorPropertiesBox() {}

ActorPropertiesBox::ActorPropertiesBox(Rectangle rect)
{
    actor = nullptr;

    rect.height += 2;
	this->rect = rect;

	scrollVec = Vector2 { 0, 0 };
    viewRec = Rectangle { 0 };
}

void ActorPropertiesBox::setDefaults()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.getType() == FILE_ACTOR) {
        this->actor = fs.getActor();
    }
}

void ActorPropertiesBox::update()
{
    FileSystemService& fs = Editor::getFileSystem();
    if (fs.getType() == FILE_ACTOR) {
        this->actor = fs.getActor();
    }
}

void ActorPropertiesBox::draw()
{
    FileSystemService& fs = Editor::getFileSystem();
    if (fs.getType() == FILE_ACTOR) {
        this->actor = fs.getActor();
    }
    if (actor == nullptr) {
        return;
    }

    ImGui::Checkbox("Collision View", &collisionViewActive);

    std::string tileSetSource = this->actor->getTileSetSource();
    tileSetSource.push_back('\0');

    ImGui::InputText("TileSet", tileSetSource.data(), tileSetSource.size());
    if (ImGui::Button("Change TileSet..", ImVec2(-1, 0))) {
        FS_Result fsResult = fs.openTileSetResource();
        if (fsResult.result == NFD_OKAY) {
            actor->setTileSet(fsResult.path);
        }
    }
}

bool ActorPropertiesBox::getCollisionViewActive()
{
    return collisionViewActive;
}
