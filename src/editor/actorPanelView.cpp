#include "actorPanelView.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"

ActorPanelView::ActorPanelView() {}

ActorPanelView::ActorPanelView(Rectangle rect)
{
	this->rect = rect;
	this->actorView = ActorView(rect);
}

void ActorPanelView::setInitial()
{

}

void ActorPanelView::update()
{
	actorView.update();
}

void ActorPanelView::draw()
{
	actorView.draw();
}