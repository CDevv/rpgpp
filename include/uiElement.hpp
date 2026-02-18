#ifndef _RPGPP_UIELEMENT_H
#define _RPGPP_UIELEMENT_H

class UIElement {
  public:
	virtual ~UIElement() = default;

	UIElement();
	virtual void update();
	virtual void draw();
};

#endif
