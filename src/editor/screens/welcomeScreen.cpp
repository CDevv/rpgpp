#include "welcomeScreen.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "editorGuiService.hpp"
#include "newFileDialog.hpp"
#include "newProjectWindow.hpp"
#include "project.hpp"
#include "translationService.hpp"
#include <filesystem>
#include <memory>

void screens::WelcomeScreen::initItems(tgui::Group::Ptr layout) {
	auto &ts = Editor::instance->getTranslations();

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
		tgui::Button::create(ts.getKey("file.new_project"));
	newProjButton->getRenderer()->setTextSize(ACTION_BUTTON_SIZE);
	const auto openProjButton =
		tgui::Button::create(ts.getKey("file.open_project"));
	openProjButton->getRenderer()->setTextSize(ACTION_BUTTON_SIZE);

	newProjectDialog = NewFileDialog::create();

	std::weak_ptr<NewFileDialog> weak_dialog = newProjectDialog;
	/*
	newProjectDialog->confirmButton->onPress([weak_dialog] {
		std::shared_ptr<NewFileDialog> dialogPtr = weak_dialog.lock();
		Project::create(

			dialogPtr->fileField->chosenPath.toStdString(),
			dialogPtr->titleField->getText().toStdString());
	});
	*/

	newProjButton->onPress([this] {
		auto childDialog = tgui::ChildWindow::create();

		newProjectDialog->init(Editor::instance->getGui().gui.get());
		newProjectDialog->fileField->setSelectingDirectory(true);
		newProjectDialog->fileLabel->setText("DIrectory..");
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