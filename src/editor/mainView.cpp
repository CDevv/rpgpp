#include "mainView.hpp"

MainView::MainView()
{
	this->panel = MainViewPanel(Rectangle {
		static_cast<float>((GetScreenWidth() - 420) / 2), static_cast<float>((GetScreenHeight() - 320) / 2),
		420, 320
	});

	this->area = TextArea(Rectangle {
		20, 20, 240, 50
	});
}

void MainView::update()
{
	panel.update();
	area.update();
}

void MainView::draw()
{
	panel.draw();
	area.draw();
}