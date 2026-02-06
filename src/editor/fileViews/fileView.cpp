#include "fileViews/fileView.hpp"
#include "actions/action.hpp"
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
	actions.push(std::move(action));
}

void FileView::undoAction() {
	if (!actions.empty()) {
		actions.top()->undo();
		actions.pop();
	}
}