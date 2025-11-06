#ifndef _RPGPP_EDITOR_PROPPANELVIEW_H
#define _RPGPP_EDITOR_PROPPANELVIEW_H

#include <raylib.h>
#include "propertiesBox.hpp"
#include "propView.hpp"

class PropPanelView {
private:
    Rectangle rect;
    PropView propView;
    PropertiesBox props;
public:
    PropPanelView();
    PropPanelView(Rectangle rect);
    void setRect(Rectangle rect);
	void setInitial();
	void update();
	void draw();
};

#endif
