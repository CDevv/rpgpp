#include "actorPanelView.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"

ActorPanelView::ActorPanelView() {}

ActorPanelView::ActorPanelView(Rectangle rect)
{
	this->rect = rect;
}

void ActorPanelView::setInitial()
{

}

void ActorPanelView::update()
{

}

void ActorPanelView::draw()
{
	FileSystemService& fs = Editor::getFileSystem();

	DrawRectangleLinesEx(rect, 1.0f, GRAY);

	//draw the actor..
	Actor* actor = fs.getActor();

	TileSet& tileSet = actor->getTileSet();
	Texture texture = tileSet.getTexture();

	auto firstAnim = actor->getAnimationsRaw().at(0);
	Vector2 firstFrame = firstAnim.at(0);
	Vector2 atlasCoords = firstFrame;

	//defaults..
    const Vector2 origin = Vector2 { 0.0f, 0.0f };
    const float rotation = 0.0f;
    const Vector2 atlasTileSize = tileSet.getTileSize();
    const float worldTileSize = 48.0f;

    Vector2 position = Vector2 { rect.x + 20, rect.y + 20 };

    //build rects
    Rectangle atlasRect = Rectangle {
        atlasCoords.x, atlasCoords.y,
        atlasTileSize.x, atlasTileSize.y
    };
    Rectangle worldRect = Rectangle {
        position.x, position.y,
        worldTileSize, worldTileSize
    };

    //draw it..
    DrawTexturePro(texture, atlasRect, worldRect, origin, rotation, WHITE);
}