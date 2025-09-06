#include "actorView.hpp"
#include "gamedata.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include <raymath.h>

ActorView::ActorView() {}

ActorView::ActorView(Rectangle rect)
{
	this->frameCounter = 0;

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

	currentAnim = 0;
	animPlaying = false;
	currentFrame = 0;
	currentAnimFrames = std::vector<Vector2>{};
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
		currentAnim = 0;
		animPlaying = false;
		currentFrame = 0;
		currentAnimFrames = std::vector<Vector2>{};
	}
}

void ActorView::update()
{
	frameCounter++;

	FileSystemService& fs = Editor::getFileSystem();

	//mouse
	mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
    mouseWorldPos = Vector2Subtract(mouseWorldPos, Vector2Scale(Vector2 {rect.x, rect.y}, 1/camera.zoom));
    mouseWorldPos = Vector2Add(mouseWorldPos, Vector2 { -1, -1 });

    collisionBox.setMouseWorldPos(mouseWorldPos);
    if (collisionViewActive) {
    	collisionBox.update();
    }

	if (fs.fileIsOpen() && fs.getActor() != nullptr) {
		Rectangle actorRect = fs.getActor()->getRect();
		camera.target = Vector2 { actorRect.width / 2, actorRect.height / 2 };

		//animation playing
		if (animPlaying) {
			if (frameCounter >= (60/2)) {
				frameCounter = 0;
				currentFrame++;

				if (currentFrame >= currentAnimFrames.size()) currentFrame = 0;
			}
		}
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

	auto firstAnim = actor->getAnimationsRaw().at(currentAnim);
	Vector2 firstFrame = firstAnim.at(currentFrame);
	Vector2 atlasCoords = firstFrame;

	//defaults..
    Vector2 origin = Vector2 { 0.0f, 0.0f };
    float rotation = 0.0f;
    Vector2 atlasTileSize = tileSet.getTileSize();
    Vector2 worldTileSize = Vector2 { atlasTileSize.x * RPGPP_DRAW_MULTIPLIER, atlasTileSize.y * RPGPP_DRAW_MULTIPLIER };

    Vector2 position = Vector2 { 0, 0 };

    //build rects
    Rectangle atlasRect = Rectangle {
        atlasCoords.x * atlasTileSize.x, atlasCoords.y * atlasTileSize.y,
        atlasTileSize.x, atlasTileSize.y
    };
    Rectangle worldRect = Rectangle {
        position.x, position.y,
        worldTileSize.x, worldTileSize.y
    };

    //draw it..
    DrawTexturePro(texture, atlasRect, worldRect, origin, rotation, WHITE);
}

void ActorView::updateData()
{
	FileSystemService& fs = Editor::getFileSystem();

	if (fs.fileIsOpen() && fs.getActor() != nullptr) {
		this->currentAnimFrames = fs.getActor()->getAnimationRaw(static_cast<Direction>(currentAnim));
		if (currentFrame >= currentAnimFrames.size()) {
			currentFrame = 0;
		}
		if (currentFrame < 0) currentFrame = 0;
	}
}

void ActorView::setCollisionActive(bool value)
{
	this->collisionViewActive = value;
}

void ActorView::setAnimation(int id)
{
	this->currentAnim = id;

	FileSystemService& fs = Editor::getFileSystem();
	if (fs.fileIsOpen() && fs.getActor() != nullptr) {
		this->currentAnimFrames = fs.getActor()->getAnimationRaw(static_cast<Direction>(id));
	}
}

void ActorView::setAnimPlaying(bool value)
{
	this->animPlaying = value;
}

void ActorView::setFrame(int frame)
{
	this->currentFrame = frame;
}

int ActorView::getFrame()
{
	return currentFrame;
}