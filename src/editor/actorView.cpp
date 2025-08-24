#include "actorView.hpp"
#include "gamedata.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include <raymath.h>

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

	this->mouseWorldPos = Vector2 { 0, 0 };

	collisionBox = CollisionBox();
	collisionViewActive = false;
}

void ActorView::setInitial()
{
	FileSystemService& fs = Editor::getFileSystem();
	Actor* actor = fs.getActor();

	if (actor != nullptr)
	{
		collisionBox.setRect(actor->getCollisionRect());
	}
}

void ActorView::update()
{
	FileSystemService& fs = Editor::getFileSystem();

	//mouse
	mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
    mouseWorldPos = Vector2Subtract(mouseWorldPos, Vector2Scale(Vector2 {rect.x, rect.y}, 1/camera.zoom));
    mouseWorldPos = Vector2Add(mouseWorldPos, Vector2 { -1, -1 });

    collisionBox.setMouseWorldPos(mouseWorldPos);
    collisionBox.update();

	if (fs.getActor() != nullptr) {
		Rectangle actorRect = fs.getActor()->getRect();
		camera.target = Vector2 { actorRect.width / 2, actorRect.height / 2 };
	}

	bool isInRect = CheckCollisionPointRec(GetMousePosition(), renderRect);

	if (GetMouseWheelMove() != 0 && isInRect) {
		camera.zoom += GetMouseWheelMove() * 0.1f;

		if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 1.0f) camera.zoom = 1.0f;
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
	if (collisionViewActive) {
    	//drawCollision();
    	collisionBox.draw();
    }

    //DrawCircleV(mouseWorldPos, 2.0f, PURPLE);

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
    Vector2 worldTileSize = Vector2 { atlasTileSize.x * RPGPP_DRAW_MULTIPLIER, atlasTileSize.y * RPGPP_DRAW_MULTIPLIER };

    Vector2 position = Vector2 { 0, 0 };

    //build rects
    Rectangle atlasRect = Rectangle {
        atlasCoords.x, atlasCoords.y,
        atlasTileSize.x, atlasTileSize.y
    };
    Rectangle worldRect = Rectangle {
        position.x, position.y,
        worldTileSize.x, worldTileSize.y
    };

    //draw it..
    DrawTexturePro(texture, atlasRect, worldRect, origin, rotation, WHITE);
}

void ActorView::drawCollision()
{
	FileSystemService& fs = Editor::getFileSystem();
	Actor* actor = fs.getActor();

	Rectangle collisionRect = actor->getCollisionRect();

    DrawRectangleRec(collisionRect, Fade(RED, 0.5f));

    Rectangle topLeft = Rectangle {
    	collisionRect.x, collisionRect.y, 4, 4
    };
    Rectangle topRight = Rectangle {
    	collisionRect.x + (collisionRect.width - 4), collisionRect.y, 4, 4
    };
    Rectangle bottomLeft = Rectangle {
    	collisionRect.x, collisionRect.y + (collisionRect.height - 4), 4, 4
    };
    Rectangle bottomRight = Rectangle {
    	collisionRect.x + (collisionRect.width - 4), collisionRect.y + (collisionRect.height - 4), 4, 4
    };

    DrawRectangleRec(topLeft, RED);
    DrawRectangleRec(topRight, RED);
    DrawRectangleRec(bottomLeft, RED);
    DrawRectangleRec(bottomRight, RED);

    if (CheckCollisionPointRec(mouseWorldPos, topLeft))
    {
    	DrawRectangleRec(topLeft, ORANGE);
    }
}

void ActorView::setCollisionActive(bool value)
{
	this->collisionViewActive = value;
}