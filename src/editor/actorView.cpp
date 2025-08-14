#include "actorView.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"

ActorView::ActorView() {}

ActorView::ActorView(Rectangle rect)
{
	this->rect = rect;
	this->renderRect = Rectangle {
		rect.x + 4, rect.y + 4,
		rect.width - 8, rect.height - 8
	};
	renderTexture = LoadRenderTexture(renderRect.width, renderRect.height);

	camera = Camera2D { 0 };
	camera.target = Vector2 { 0, 0 };
	camera.offset = Vector2 { renderRect.width / 2, renderRect.height / 2 };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
}

void ActorView::update()
{
	FileSystemService& fs = Editor::getFileSystem();

	if (fs.getActor() != nullptr) {
		Rectangle actorRect = fs.getActor()->getRect();
		camera.target = Vector2 { actorRect.width / 2, actorRect.height / 2 };
	}

	if (GetMouseWheelMove() != 0) {
		camera.zoom += GetMouseWheelMove() * 0.1f;

		if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.5f) camera.zoom = 0.5f;
	}
}

void ActorView::draw()
{
	DrawRectangleLinesEx(rect, 1.0f, GRAY);

	//texture mode
	BeginTextureMode(renderTexture);
	ClearBackground(RAYWHITE);
	BeginMode2D(camera);

	drawActor();

	EndMode2D();
	EndTextureMode();

	Rectangle cameraRect = Rectangle {
        0, 0,
        static_cast<float>(renderTexture.texture.width), static_cast<float>(-renderTexture.texture.height)
    };
	DrawTextureRec(renderTexture.texture, cameraRect, Vector2 { renderRect.x, renderRect.y }, WHITE);
}

void ActorView::drawActor()
{
	FileSystemService& fs = Editor::getFileSystem();

	//draw the actor..
	Actor* actor = fs.getActor();

	TileSet& tileSet = actor->getTileSet();
	Texture texture = tileSet.getTexture();

	auto firstAnim = actor->getAnimationsRaw().at(0);
	Vector2 firstFrame = firstAnim.at(0);
	Vector2 atlasCoords = firstFrame;

	//defaults..
    Vector2 origin = Vector2 { 0.0f, 0.0f };
    float rotation = 0.0f;
    Vector2 atlasTileSize = tileSet.getTileSize();
    float worldTileSize = 48.0f;

    Vector2 position = Vector2 { 0, 0 };

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