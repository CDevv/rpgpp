#include "actorPanelView.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"

ActorPanelView::ActorPanelView() {}

ActorPanelView::ActorPanelView(Rectangle rect)
{
	this->rect = rect;

	Rectangle actorViewRect = Rectangle {
		rect.x, rect.y,
		rect.width - 260, rect.height
	};
	this->actorView = ActorView(actorViewRect);

	Rectangle propsRect = Rectangle {
		rect.x + actorViewRect.width + 4, rect.y,
		260 - 4, rect.height
	};
	this->props = PropertiesBox(propsRect);
}

void ActorPanelView::setInitial()
{
	props.setDefaults();
}

void ActorPanelView::update()
{
	props.update();
	actorView.setCollisionActive(props.getActorProps().getCollisionViewActive());
	actorView.update();
}

void ActorPanelView::draw()
{
	props.draw();
	actorView.draw();
}