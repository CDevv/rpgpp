#include "fileViews/fileView.hpp"
#include "actions/action.hpp"
#include <cstdio>
#include <memory>
#include <utility>

FileView::FileView() {}

FileView::~FileView() {}

void FileView::addWidgets(tgui::Group::Ptr layout) {
	for (auto widget : widgetContainer) {
		layout->add(widget);
	}
}

void FileView::pushAction(std::unique_ptr<Action> action) {
	if (action->executeOnAdd) {
		action->execute();
	}

	past.push(std::move(action));
}

void FileView::undoAction() {
	if (!past.empty()) {
		past.top()->undo();

		future.push(std::move(past.top()));
		past.pop();
	}
}

void FileView::redoAction() {
	if (!future.empty()) {
		future.top()->execute();

		past.push(std::move(future.top()));
		future.pop();
	}
}