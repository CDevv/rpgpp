#include "collisionbox.hpp"
#include "gamedata.hpp"
#include <raymath.h>
#include <cstdio>
#include "editor.hpp"
#include "fileSystemService.hpp"

CollisionBox::CollisionBox() 
{
	this->rect = Rectangle { 0, 0, 0, 0 };
	this->mouseWorldPos = Vector2 { 0, 0 };

	topLeft = Rectangle { 0, 0, 0, 0 };
	topRight = Rectangle { 0, 0, 0, 0 };
	bottomLeft = Rectangle { 0, 0, 0, 0 };
	bottomRight = Rectangle { 0, 0, 0, 0 };

	oldMousePos = Vector2 { 0, 0 };
	newRect = rect;
	mouseDelta = Vector2 { 0, 0 };
	holdingMouse = false;
	resizeMode = RESIZE_TOP_LEFT;
	mouseOffset = Vector2 { 0, 0 };
}

CollisionBox::CollisionBox(Rectangle rect) 
{
	this->rect = rect;
	this->mouseWorldPos = Vector2 { 0, 0 };

	topLeft = Rectangle {
    	rect.x, rect.y, 4, 4
    };
    topRight = Rectangle {
    	rect.x + (rect.width - 4), rect.y, 4, 4
    };
    bottomLeft = Rectangle {
    	rect.x, rect.y + (rect.height - 4), 4, 4
    };
    bottomRight = Rectangle {
    	rect.x + (rect.width - 4), rect.y + (rect.height - 4), 4, 4
    };
}

void CollisionBox::setMouseWorldPos(Vector2 pos)
{
	this->mouseWorldPos = pos;
}

void CollisionBox::setRect(Rectangle rect)
{
	this->rect = rect;

	topLeft = Rectangle {
    	rect.x, rect.y, 4, 4
    };
    topRight = Rectangle {
    	rect.x + (rect.width - 4), rect.y, 4, 4
    };
    bottomLeft = Rectangle {
    	rect.x, rect.y + (rect.height - 4), 4, 4
    };
    bottomRight = Rectangle {
    	rect.x + (rect.width - 4), rect.y + (rect.height - 4), 4, 4
    };
}

Rectangle CollisionBox::getRect()
{
	return this->rect;
}

void CollisionBox::update()
{

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
	{
		if (CheckCollisionPointRec(mouseWorldPos, topLeft))
		{
			holdingMouse = true;
			resizeMode = RESIZE_TOP_LEFT;
		}
		else if (CheckCollisionPointRec(mouseWorldPos, topRight))
		{
			holdingMouse = true;
			resizeMode = RESIZE_TOP_RIGHT;
		}
		else if (CheckCollisionPointRec(mouseWorldPos, bottomLeft))
		{
			holdingMouse = true;
			resizeMode = RESIZE_BOTTOM_LEFT;
		}
		else if (CheckCollisionPointRec(mouseWorldPos, bottomRight))
		{
			holdingMouse = true;
			resizeMode = RESIZE_BOTTOM_RIGHT;
		}
		else if (CheckCollisionPointRec(mouseWorldPos, rect))
		{
			holdingMouse = true;
			resizeMode = RESIZE_MOVE;
			mouseOffset = Vector2 { mouseWorldPos.x - rect.x, mouseWorldPos.y - rect.y };
		}
	}

	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
	{
		if (holdingMouse) {
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);

			holdingMouse = false;
			resizeMode = RESIZE_TOP_LEFT;

			this->setRect(newRect);

			FileSystemService& fs = Editor::getFileSystem();
			Actor* actor = fs.getActor();

			if (actor != nullptr)
			{
				actor->setCollisionRect(rect);
			}
		}
	}

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		oldMousePos = mouseWorldPos;

		float widthChangeTopRight = -1 * ((rect.x + rect.width) - oldMousePos.x);
		float heightChangeTopRight = (rect.y - oldMousePos.y);

		float widthChangeBottomLeft = (rect.x - oldMousePos.x);
		float heightChangeBottomLeft = -1 * ((rect.y + rect.height) - oldMousePos.y);

		float widthChangeBottomRight = -1 * ((rect.x + rect.width) - oldMousePos.x);
		float heigthChangeBottomRight = -1 * ((rect.y + rect.height) - oldMousePos.y);

		switch (resizeMode) {
		case RESIZE_TOP_LEFT:
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);

			newRect = Rectangle {
				floor(mouseWorldPos.x / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
				floor(mouseWorldPos.y / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER, 
				floor((rect.width + (rect.x - oldMousePos.x)) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER + RPGPP_DRAW_MULTIPLIER,
				floor((rect.height + (rect.y - oldMousePos.y)) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER + RPGPP_DRAW_MULTIPLIER
			};
			break;
		case RESIZE_TOP_RIGHT:
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);

			newRect = Rectangle {
				floor(rect.x / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
				floor(mouseWorldPos.y / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER, 
				floor((rect.width + widthChangeTopRight) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
				floor((rect.height + heightChangeTopRight) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER + RPGPP_DRAW_MULTIPLIER
			};
			break;
		case RESIZE_BOTTOM_LEFT:
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);

			newRect = Rectangle {
				floor((mouseWorldPos.x)/ RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
				floor(rect.y / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
				floor((rect.width + widthChangeBottomLeft) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
				floor((rect.height + heightChangeBottomLeft) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER + RPGPP_DRAW_MULTIPLIER
			};
			break;
		case RESIZE_BOTTOM_RIGHT:
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);

			newRect = Rectangle {
				floor(rect.x / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
				floor(rect.y / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
				floor((rect.width + widthChangeBottomRight) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
				floor((rect.height + heigthChangeBottomRight) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER + RPGPP_DRAW_MULTIPLIER
			};
			break;
		case RESIZE_MOVE:
			SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);

			newRect = Rectangle {
				floor((oldMousePos.x - mouseOffset.x) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
				floor((oldMousePos.y - mouseOffset.y) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
				rect.width,
				rect.height
			};
			break;
		}
	}
}

void CollisionBox::draw()
{
	DrawRectangleRec(rect, Fade(RED, 0.5f));

	DrawRectangleRec(topLeft, RED);
    DrawRectangleRec(topRight, RED);
    DrawRectangleRec(bottomLeft, RED);
    DrawRectangleRec(bottomRight, RED);

    if (CheckCollisionPointRec(mouseWorldPos, topLeft))
    {
    	DrawRectangleRec(topLeft, ORANGE);
    }
    else if (CheckCollisionPointRec(mouseWorldPos, topRight))
    {
    	DrawRectangleRec(topRight, ORANGE);
    }
    else if (CheckCollisionPointRec(mouseWorldPos, bottomLeft))
    {
    	DrawRectangleRec(bottomLeft, ORANGE);
    }
    else if (CheckCollisionPointRec(mouseWorldPos, bottomRight))
    {
    	DrawRectangleRec(bottomRight, ORANGE);
    }

    if (holdingMouse) 
    {
		DrawRectangleLinesEx(newRect, 1.0f, GRAY);
    }
}