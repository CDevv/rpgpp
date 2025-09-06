#include "mainView.hpp"

MainView::MainView()
{
	this->panel = MainViewPanel(Rectangle {
		static_cast<float>((GetScreenWidth() - 420) / 2), static_cast<float>((GetScreenHeight() - 320) / 2),
		420, 320
	});
}

void MainView::update()
{
	panel.update();
}

void MainView::draw()
{
	panel.draw();
}