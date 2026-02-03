#include "newProjectWindow.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "widgets/fileChooser.hpp"
#include <memory>

NewProjectWindow::NewProjectWindow(const char *typeName, bool initRenderer) {}

void NewProjectWindow::init(tgui::Gui *gui) {
	window = tgui::ChildWindow::create("New..");
	window->setSize(320, 220);

	auto panel = tgui::Panel::create();
	panel->getRenderer()->setPadding({8});
	window->add(panel);

	auto vertLayout = tgui::GrowVerticalLayout::create();
	panel->add(vertLayout);

	auto titleLabel = tgui::Label::create("Title");
	vertLayout->add(titleLabel);

	titleField = tgui::EditBox::create();
	titleField->setSize("100%", 24);
	titleField->setDefaultText("Title..");
	vertLayout->add(titleField);

	auto gap = tgui::Label::create();
	gap->setSize("100%", 24);
	vertLayout->add(gap);

	fileLabel = tgui::Label::create("File..");
	vertLayout->add(fileLabel);

	fileField = FileChooser::create();
	fileField->setSize({"100%", 24});
	vertLayout->add(fileField);

	confirmButton = tgui::Button::create("Confirm");
	confirmButton->setSize(108, 24);
	confirmButton->setPosition("100% - 108 - 8", "100% - 24 - 8");

	auto weakTitle = std::weak_ptr<tgui::EditBox>(titleField);
	auto weakPath = std::weak_ptr<FileChooser>(fileField);
	auto weakWindow = std::weak_ptr<tgui::ChildWindow>(window);

	window->add(confirmButton);

	cancelButton = tgui::Button::create("Cancel");
	cancelButton->setSize(108, 24);
	cancelButton->setPosition("100% - 108 - 8 - 108 - 8", "100% - 24 - 8");

	cancelButton->onPress([this] { window->close(); });
	window->add(cancelButton);

	gui->add(window);
}

NewProjectWindow::Ptr NewProjectWindow::create() {
	return std::make_shared<NewProjectWindow>();
}

NewProjectWindow::Ptr NewProjectWindow::create(const tgui::String &title) {
	auto ptr = std::make_shared<NewProjectWindow>();
	ptr->window->setTitle(title);
	return ptr;
}

void NewProjectWindow::setFieldTitle(const tgui::String &title) {
	titleField->setDefaultText(title);
}

void NewProjectWindow::setFileFieldTitle(const tgui::String &title) {
	fileField->chosenPathLabel->setText(title);
}

void NewProjectWindow::setPathFilters(
	std::vector<std::pair<tgui::String, std::vector<tgui::String>>>
		pathFilters) {
	fileField->pathFilters = pathFilters;
}

void NewProjectWindow::updateSize(const tgui::Layout2d &size) {
	window->setSize(size.x, size.y);
}
