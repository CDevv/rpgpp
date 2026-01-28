#include "fileViews/fileView.hpp"

FileView::FileView() {}

FileView::~FileView() {}

void FileView::addWidgets(tgui::Group::Ptr layout) {
	for (auto widget : widgetContainer) {
		layout->add(widget);
	}
}