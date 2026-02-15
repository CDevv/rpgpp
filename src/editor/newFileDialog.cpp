#include "newFileDialog.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "services/translationService.hpp"
#include "widgets/fileChooser.hpp"
#include <memory>

NewFileDialog::NewFileDialog(const char *typeName, bool initRenderer) {}

void NewFileDialog::init(tgui::Gui *gui) {
   	TranslationService &tService = Editor::instance->getTranslations();
	window = tgui::ChildWindow::create(tService.getKey("dialog.new_file.title"));
	window->setSize(320, 220);

	auto panel = tgui::Panel::create();
	panel->getRenderer()->setPadding({PADDING});
	window->add(panel);

	auto vertLayout = tgui::GrowVerticalLayout::create();
	panel->add(vertLayout);

	auto titleLabel = tgui::Label::create(tService.getKey("dialog.new_file.name"));
	vertLayout->add(titleLabel);

	titleField = tgui::EditBox::create();
	titleField->setSize("100%", FIELD_H);
	titleField->setDefaultText("Title..");
	vertLayout->add(titleField);

	auto gap = tgui::Label::create();
	gap->setSize("100%", FIELD_H);
	vertLayout->add(gap);

	fileLabel = tgui::Label::create(tService.getKey("dialog.new_file.file"));
	vertLayout->add(fileLabel);

	fileField = FileChooser::create();
	fileField->setSize({"100%", FIELD_H});
	vertLayout->add(fileField);

	confirmButton = tgui::Button::create(tService.getKey("dialog.new_file.confirm"));
	confirmButton->setSize(BUTTON_W, BUTTON_H);
	confirmButton->setPosition(
	    TextFormat("100%% - %d - %d", BUTTON_W, PADDING),
		TextFormat("100%% - %d - %d", BUTTON_H, PADDING)
	);

	auto weakTitle = std::weak_ptr<tgui::EditBox>(titleField);
	auto weakPath = std::weak_ptr<FileChooser>(fileField);
	auto weakWindow = std::weak_ptr<tgui::ChildWindow>(window);

	window->add(confirmButton);

	cancelButton = tgui::Button::create(tService.getKey("dialog.new_file.cancel"));
	cancelButton->setSize(BUTTON_W, BUTTON_H);
	cancelButton->setPosition(
	    TextFormat("100%% - %d - %d", BUTTON_W + PADDING, BUTTON_W + PADDING),
		TextFormat("100%% - %d - %d", BUTTON_H, PADDING)
	);

	cancelButton->onPress([this] { window->close(); });
	window->add(cancelButton);

	window->setPosition({TextFormat("50%% - %d", window->getSize().x), TextFormat("50%% - %d", window->getSize().y)});

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
	fileField->chosenPathLabel->setText(title);
}

void NewFileDialog::setPathFilters(
	std::vector<std::pair<tgui::String, std::vector<tgui::String>>>
		pathFilters) {
	fileField->pathFilters = pathFilters;
}

void NewFileDialog::updateSize(const tgui::Layout2d &size) {
	window->setSize(size.x, size.y);
}
