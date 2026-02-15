#include "screens/welcomeScreen.hpp"
#include "services/editorGuiService.hpp"
#include "services/translationService.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "widgets/newProjectWindow.hpp"
#include "project.hpp"
#include <memory>

void screens::WelcomeScreen::initItems(tgui::Group::Ptr layout) {
	auto &ts = Editor::instance->getTranslations();

	if (!Editor::instance->getGui().menuBar.expired()) {
		auto menuBarPtr = Editor::instance->getGui().menuBar.lock();
		menuBarPtr->setMenuItemEnabled(
			{ts.getKey("menu.file"), ts.getKey("menu.file.save_file")}, false);
		menuBarPtr->setMenuItemEnabled(
			{ts.getKey("menu.edit"), ts.getKey("menu.edit.undo")}, false);
		menuBarPtr->setMenuItemEnabled(
			{ts.getKey("menu.edit"), ts.getKey("menu.edit.redo")}, false);
	}

	const auto verticalLayout = tgui::GrowVerticalLayout::create();
	verticalLayout->setPosition({"50%", "50%"});
	verticalLayout->setWidth(640);
	verticalLayout->setOrigin(0.5, 0.5);
	verticalLayout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	EditorGuiService::createLogoCenter(verticalLayout);

	const auto headerLabel =
		tgui::Label::create(ts.getKey("screen.starting.get_started"));
	headerLabel->getRenderer()->setTextSize(32);
	headerLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	verticalLayout->add(headerLabel);

	const auto introLabel =
		tgui::Label::create(ts.getKey("screen.starting.description"));
	introLabel->getRenderer()->setTextSize(16);
	introLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	introLabel->setSize({0, 81});
	verticalLayout->add(introLabel);

	const auto newProjButton =
		tgui::Button::create(ts.getKey("menu.file.new_project"));
	newProjButton->getRenderer()->setTextSize(ACTION_BUTTON_SIZE);
	const auto openProjButton =
		tgui::Button::create(ts.getKey("menu.file.open_project"));
	openProjButton->getRenderer()->setTextSize(ACTION_BUTTON_SIZE);

	newProjectDialog = NewProjectWindow::create();

	newProjButton->onPress([this] {
		auto childDialog = tgui::ChildWindow::create();

		newProjectDialog->init(Editor::instance->getGui().gui.get());
		newProjectDialog->fileField->setSelectingDirectory(true);
		newProjectDialog->confirmButton->onPress([this] {
			std::string title =
				newProjectDialog->titleField->getText().toStdString();
			std::string dirPath =
				newProjectDialog->fileField->getChosenPath().toStdString();
			if (!title.empty() && !dirPath.empty()) {
				Project::create(dirPath, title);
			}
		});
	});

	openProjButton->onPress(
		[] { Editor::instance->getFs().promptOpenProject(); });

	verticalLayout->add(newProjButton);
	verticalLayout->add(openProjButton);

	layout->add(verticalLayout);
}
