#include "widgets/newProjectWindow.hpp"

#include <memory>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "bindTranslation.hpp"
#include "editor.hpp"
#include "services/translationService.hpp"
#include "widgets/fileChooser.hpp"
NewProjectWindow::NewProjectWindow(const char *typeName, bool initRenderer) {}

void NewProjectWindow::init(tgui::Gui *gui) {
	TranslationService &tService = Editor::instance->getTranslations();

	window = tgui::ChildWindow::create();
	bindTranslation<tgui::ChildWindow>(window, "dialog.new_project.title", &tgui::ChildWindow::setTitle);
	window->setSize(320, 220);

	auto panel = tgui::Panel::create();
	panel->getRenderer()->setPadding({PADDING});
	window->add(panel);

	auto vertLayout = tgui::GrowVerticalLayout::create();
	panel->add(vertLayout);

	auto titleLabel = tgui::Label::create();
	bindTranslation<tgui::Label>(titleLabel, "dialog.new_project.name", &tgui::Label::setText);
	vertLayout->add(titleLabel);

	titleField = tgui::EditBox::create();
	titleField->setSize("100%", FIELD_H);
	vertLayout->add(titleField);

	auto gap = tgui::Label::create();
	gap->setSize("100%", FIELD_H);
	vertLayout->add(gap);

	fileLabel = tgui::Label::create();
	bindTranslation<tgui::Label>(fileLabel, "dialog.new_project.folder", &tgui::Label::setText);
	vertLayout->add(fileLabel);

	fileField = FileChooser::create();
	fileField->setSize({"100%", FIELD_H});
	vertLayout->add(fileField);

	confirmButton = tgui::Button::create();
	bindTranslation<tgui::Button>(confirmButton, "dialog.new_project.confirm", &tgui::Button::setText);
	confirmButton->setSize(BUTTON_W, BUTTON_H);
	confirmButton->setPosition(tgui::Layout("100%") - BUTTON_W - PADDING, tgui::Layout("100%") - BUTTON_H - PADDING);

	window->add(confirmButton);

	cancelButton = tgui::Button::create();
	bindTranslation<tgui::Button>(cancelButton, "dialog.new_project.cancel", &tgui::Button::setText);
	cancelButton->setSize(BUTTON_W, BUTTON_H);
	cancelButton->setPosition(tgui::bindLeft(confirmButton) - BUTTON_W - PADDING,
							  tgui::Layout("100%") - BUTTON_H - PADDING);

	cancelButton->onPress([this] { window->close(); });
	window->add(cancelButton);

	gui->add(window);
}

NewProjectWindow::Ptr NewProjectWindow::create() { return std::make_shared<NewProjectWindow>(); }

NewProjectWindow::Ptr NewProjectWindow::create(const tgui::String &title) {
	auto ptr = std::make_shared<NewProjectWindow>();
	ptr->window->setTitle(title);
	return ptr;
}

void NewProjectWindow::setFieldTitle(const tgui::String &title) { titleField->setDefaultText(title); }

void NewProjectWindow::setFileFieldTitle(const tgui::String &title) { fileField->chosenPathLabel->setText(title); }

void NewProjectWindow::setPathFilters(std::vector<std::pair<tgui::String, std::vector<tgui::String>>> pathFilters) {
	fileField->pathFilters = pathFilters;
}

void NewProjectWindow::updateSize(const tgui::Layout2d &size) { window->setSize(size.x, size.y); }
