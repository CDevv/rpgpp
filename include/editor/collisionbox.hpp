#ifndef _RPGPP_EDITOR_COLLISIONBOX_H
#define _RPGPP_EDITOR_COLLISIONBOX_H

#include <raylib.h>

class CollisionBox {
private:
	Rectangle rect;
	Vector2 mouseWorldPos;
	Rectangle topLeft;
	Rectangle topRight;
	Rectangle bottomLeft;
	Rectangle bottomRight;
	Vector2 oldMousePos;
	Rectangle newRect;
	Vector2 mouseDelta;
	bool holdingMouse;
public:
	CollisionBox();
	CollisionBox(Rectangle rect);
	void setMouseWorldPos(Vector2 pos);
	void setRect(Rectangle rect);
	Rectangle getRect();
	void update();
	void draw();
};

#endif