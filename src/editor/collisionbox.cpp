#include "collisionbox.hpp"
#include "gamedata.hpp"
#include <raymath.h>
#include <cstdio>

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
		}
	}
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
	{
		if (holdingMouse) {
			holdingMouse = false;

			this->setRect(newRect);
		}
	}

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		oldMousePos = mouseWorldPos;
		newRect = Rectangle {
			floor(mouseWorldPos.x / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER,
			floor(mouseWorldPos.y / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER, 
			floor((rect.width + (rect.x - oldMousePos.x)) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER + RPGPP_DRAW_MULTIPLIER,
			floor((rect.height + (rect.y - oldMousePos.y)) / RPGPP_DRAW_MULTIPLIER) * RPGPP_DRAW_MULTIPLIER + RPGPP_DRAW_MULTIPLIER
		};
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

    if (holdingMouse) 
    {
		DrawRectangleLinesEx(newRect, 1.0f, GRAY);
    }
}