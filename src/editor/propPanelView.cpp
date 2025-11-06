#include "propPanelView.hpp"
#include "editor.hpp"
#include "projectFile.hpp"
#include "propView.hpp"
#include "propertiesBox.hpp"
#include <cstdio>

PropPanelView::PropPanelView() {}

PropPanelView::PropPanelView(Rectangle rect)
{
    this->rect = rect;
    propView = PropView(rect);
    props = PropertiesBox(rect);
}

void PropPanelView::setRect(Rectangle rect)
{
    this->rect = rect;
    Rectangle viewRect = Rectangle {
        rect.x, rect.y,
        rect.width * 0.7f, rect.height
    };
    propView.setRect(viewRect);

    props.setRect(Rectangle {
        viewRect.x + (rect.width * 0.7f) + 8, viewRect.y,
        (rect.width * 0.3f) - 8, rect.height
    });
}

void PropPanelView::setInitial()
{
    propView.setInitial();
    props.setDefaults();
}

void PropPanelView::update()
{
    if (Editor::getFileSystem().isAvailable(FILE_PROP)) {
        props.update();
    }

    propView.setCollisionBoxActive(props.getPropProps().getCollisionViewActive());
    propView.update();
}

void PropPanelView::draw()
{
    propView.draw();
    props.draw();
}
