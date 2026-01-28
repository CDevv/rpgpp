#include "newFileDialog.hpp"
#include "TGUI/Loading/Theme.hpp"
#include "TGUI/Renderers/ChildWindowRenderer.hpp"
#include "TGUI/String.hpp"
#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "fileField.hpp"
#include "raylib.h"
#include <cstdio>
#include <memory>

void NewFileDialog::init(tgui::Gui *gui) {
	window = tgui::ChildWindow::create("New..");
	window->setSize(320, 220);

	auto panel = tgui::Panel::create();
	panel->getRenderer()->setPadding({8});
	window->add(panel);

	auto vertLayout = tgui::GrowVerticalLayout::create();
	panel->add(vertLayout);

	titleField = tgui::EditBox::create();
	titleField->setSize("100%", 24);
	titleField->setDefaultText("Title..");
	vertLayout->add(titleField);

	fileField = FileField::create();
	fileField->setSize({"100%", 24});
	vertLayout->add(fileField);

	confirmButton = tgui::Button::create("Confirm");
	confirmButton->setSize(108, 24);
	confirmButton->setPosition("100% - 108 - 8", "100% - 24 - 8");

	auto weakTitle = std::weak_ptr<tgui::EditBox>(titleField);
	auto weakPath = std::weak_ptr<FileField>(fileField);
	auto weakWindow = std::weak_ptr<tgui::ChildWindow>(window);

	window->add(confirmButton);

	cancelButton = tgui::Button::create("Cancel");
	cancelButton->setSize(108, 24);
	cancelButton->setPosition("100% - 108 - 8 - 108 - 8", "100% - 24 - 8");

	cancelButton->onPress([this] { window->close(); });
	window->add(cancelButton);

	gui->add(window);
}

NewFileDialog::Ptr NewFileDialog::create() {
	return std::make_shared<NewFileDialog>();
}

NewFileDialog::Ptr NewFileDialog::create(const tgui::String &title) {
	auto ptr = std::make_shared<NewFileDialog>();
	ptr->window->setTitle(title);
	return ptr;
}

void NewFileDialog::setFieldTitle(const tgui::String &title) {
	titleField->setDefaultText(title);
}

void NewFileDialog::setFileFieldTitle(const tgui::String &title) {
	fileField->label->setText(title);
}

void NewFileDialog::setPathFilters(
	std::vector<std::pair<tgui::String, std::vector<tgui::String>>>
		pathFilters) {
	fileField->pathFilters = pathFilters;
}

void NewFileDialog::updateSize(const tgui::Layout2d &size) {
	window->setSize(size.x, size.y);
}