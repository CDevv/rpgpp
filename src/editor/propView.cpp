#include "propView.hpp"
#include "collisionbox.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "projectFile.hpp"
#include "prop.hpp"
#include <raylib.h>
#include <raymath.h>

PropView::PropView() {}

PropView::PropView(Rectangle rect)
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

	collisionBox = CollisionBox();
	collisionBoxActive = false;
}

void PropView::setRect(Rectangle rect)
{
    this->rect = rect;
    this->renderRect = Rectangle {
		rect.x + 4, rect.y + 4,
		rect.width - 8, rect.height - 8
	};

	camera.offset = Vector2 { renderRect.width / 2, renderRect.height / 2 };
}

void PropView::setInitial()
{
    FileSystemService& fs = Editor::getFileSystem();

    if (fs.isAvailable(FILE_PROP)) {
	    Prop* prop = fs.getCurrentFile()->getData<Prop>();

		Rectangle atlasRect = prop->getAtlasRect();
		camera.target = Vector2 {
		    (atlasRect.width * RPGPP_DRAW_MULTIPLIER) / 2,
			(atlasRect.height * RPGPP_DRAW_MULTIPLIER) / 2
		};

		collisionBox.setRect(prop->getWorldCollisionRect());
	}
}

void PropView::setCollisionBoxActive(bool value)
{
    this->collisionBoxActive = value;
}

void PropView::update()
{
    //mouse
	Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
    mouseWorldPos = Vector2Subtract(mouseWorldPos, Vector2Scale(Vector2 {rect.x, rect.y}, 1/camera.zoom));
    mouseWorldPos = Vector2Add(mouseWorldPos, Vector2 { -1, -1 });

    collisionBox.setMouseWorldPos(mouseWorldPos);
    if (collisionBoxActive) {
        collisionBox.update();
    }

    bool isInRect = CheckCollisionPointRec(GetMousePosition(), renderRect);
    if (GetMouseWheelMove() != 0 && isInRect) {
		camera.zoom += GetMouseWheelMove() * 0.1f;

		if (camera.zoom > 3.0f) {
		    camera.zoom = 3.0f;
		} else if (camera.zoom < 1.0f) {
            camera.zoom = 1.0f;
        }
	}
}

void PropView::draw()
{
    if ((renderTexture.texture.width != static_cast<int>(renderRect.width)) ||
		(renderTexture.texture.height != static_cast<int>(renderRect.height))) {

		UnloadRenderTexture(renderTexture);
		renderTexture = LoadRenderTexture(renderRect.width, renderRect.height);
	}

    DrawRectangleLinesEx(rect, 1.0f, GRAY);

    FileSystemService& fs = Editor::getFileSystem();
    EngineFileType fileType = fs.getCurrentFile()->getFileType();

    BeginTextureMode(renderTexture);
	ClearBackground(RAYWHITE);
	BeginMode2D(camera);

	if (fs.isAvailable(FILE_PROP)) {
	    Prop* prop = fs.getCurrentFile()->getData<Prop>();

		Texture2D texture = prop->getTexture();
		Rectangle atlasRect = prop->getAtlasRect();
		Vector2 worldPos = Vector2 { 0, 0 };
        Rectangle dest = Rectangle {
            worldPos.x, worldPos.y,
            atlasRect.width * RPGPP_DRAW_MULTIPLIER, atlasRect.height * RPGPP_DRAW_MULTIPLIER
        };
        DrawTexturePro(texture, atlasRect, dest, Vector2 { 0, 0 }, 0.0f, WHITE);

        if (collisionBoxActive) {
            collisionBox.draw();
        }
	}

	EndMode2D();
	EndTextureMode();

    Rectangle cameraRect = Rectangle {
           0, 0,
           static_cast<float>(renderTexture.texture.width), static_cast<float>(-renderTexture.texture.height)
    };
	DrawTextureRec(renderTexture.texture, cameraRect, Vector2 { renderRect.x, renderRect.y }, WHITE);
}
