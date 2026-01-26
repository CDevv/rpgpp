#include "screens/projectScreen.hpp"
#include "TGUI/Color.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalWrap.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "editor.hpp"
#include "fileSystemService.hpp"
#include "newFileDialog.hpp"
#include "projectFile.hpp"
#include "projectFileVisitor.hpp"
#include "raylib.h"
#include <cstdio>
#include <memory>
#include <utility>
#include <vector>

void screens::ProjectScreen::initItems(tgui::Group::Ptr layout) {
	openedFiles = std::vector<std::unique_ptr<ProjectFile>>{};
	fileVisitor = std::make_unique<ProjectFileVisitor>();

	auto toolBar = createToolBar();

	auto fileView = tgui::Group::create({"100% - 264", "100% - 54"});
	fileView->setPosition(264, 54);
	layout->add(fileView);
	this->fileViewGroup = fileView;

	auto label = tgui::Label::create("Test");
	fileView->add(label);

	auto panel = createResourcesList(fileView);

	layout->add(panel);
	layout->add(toolBar);

	// Maximize when a project is opened
	SetWindowState(FLAG_WINDOW_MAXIMIZED);
}

void screens::ProjectScreen::addFileView(EngineFileType fileType,
										 const std::string &path) {
	fileViewGroup->removeAllWidgets();
	std::unique_ptr<ProjectFile> projectFile =
		fileVisitor->visit(fileType, path);
	projectFile->initUi(fileViewGroup);
	openedFiles.push_back(std::move(projectFile));
}

tgui::HorizontalWrap::Ptr screens::ProjectScreen::createToolBar() {
	auto toolBar = tgui::HorizontalWrap::create({"100%", 54});
	toolBar->setPosition(0, 0);
	toolBar->getRenderer()->setSpaceBetweenWidgets(8.0f);
	toolBar->getRenderer()->setPadding(8);

	auto barSize = toolBar->getSize().y;

	auto project = Editor::instance->getProject();

	auto label = tgui::Label::create("text.");
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	label->setTextSize(16);
	label->setSize(256, "100%");

	if (project == nullptr) {
		label->setText("uhh, nullptr");
	} else {
		label->setText(project->getTitle());
	}

	toolBar->add(label);

	auto &fs = Editor::instance->getFs();

	auto playBtn = tgui::BitmapButton::create();
	auto playtestImg = tgui::Texture(fs.getResourcePath("playtest.png"));
	playBtn->setImage(playtestImg);
	playBtn->setSize({barSize, "100%"});
	toolBar->add(playBtn);

	auto buildBtn = tgui::BitmapButton::create();
	auto buildImg = tgui::Texture(fs.getResourcePath("build.png"));
	buildBtn->setImage(buildImg);
	buildBtn->setSize({barSize, "100%"});
	toolBar->add(buildBtn);

	return toolBar;
}

void screens::ProjectScreen::addResourceButtons(
	EngineFileType fileType, tgui::GrowVerticalLayout::Ptr vertLayout) {
	auto project = Editor::instance->getProject();

	vertLayout->removeAllWidgets();

	for (auto filePath : project->getPaths(fileType)) {
		auto fileBtn = tgui::Button::create(GetFileName(filePath.c_str()));
		fileBtn->setSize("100%", 36);
		fileBtn->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0));
		fileBtn->onPress(
			[this, fileType, filePath] { addFileView(fileType, filePath); });

		vertLayout->add(fileBtn);
	}
}

tgui::Group::Ptr
screens::ProjectScreen::createResourcesList(tgui::Group::Ptr fileViewGroup) {
	auto project = Editor::instance->getProject();

	auto group = tgui::Group::create({"264", "100% - 54"});
	group->setPosition(0, 54);

	auto resourceChoose = tgui::ComboBox::create();
	resourceChoose->setPosition(0, 0);
	resourceChoose->setSize("100%", 54);
	resourceChoose->addMultipleItems({"TileSets", "Maps"});
	resourceChoose->setSelectedItem("Maps");
	group->add(resourceChoose);

	auto createResourceBtn = tgui::Button::create("New..");
	createResourceBtn->setPosition(0, 54);
	createResourceBtn->setSize("100%", 24);
	createResourceBtn->onPress([] {
		/*
		auto childDialog = tgui::ChildWindow::create("TestDialog");

		auto vertLayout = tgui::GrowVerticalLayout::create();

		auto textEdit1 = tgui::EditBox::create();
		textEdit1->setSize("100%", 24);
		textEdit1->setDefaultText("Title.");

		vertLayout->add(textEdit1);

		auto textEdit2 = tgui::EditBox::create();
		textEdit2->setSize("100%", 24);
		textEdit2->setDefaultText("Location");

		vertLayout->add(textEdit2);

		childDialog->add(vertLayout);
		*/

		auto childDialog = NewFileDialog::create();
		childDialog->callback = [](std::string title, std::string path) {
			printf("%s \n", path.c_str());
		};

		childDialog->init(Editor::instance->getGui().gui.get());
	});
	group->add(createResourceBtn);

	auto panel = tgui::ScrollablePanel::create({"100%", "100% - 54"});
	panel->setPosition(0, 54 + 24);
	panel->getVerticalScrollbar()->setPolicy(
		tgui::Scrollbar::Policy::Automatic);
	panel->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
	panel->getRenderer()->setBackgroundColor(
		tgui::Color::applyOpacity(tgui::Color::Black, 0.2));

	group->add(panel);

	auto vertLayout = tgui::GrowVerticalLayout::create();
	panel->add(vertLayout);

	resourceChoose->onItemSelect([this, vertLayout, &fileViewGroup](int index) {
		EngineFileType currentFileType = static_cast<EngineFileType>(index);
		addResourceButtons(currentFileType, vertLayout);
	});

	if (project != nullptr) {
		EngineFileType currentFileType =
			static_cast<EngineFileType>(resourceChoose->getSelectedItemIndex());
		addResourceButtons(currentFileType, vertLayout);
	}

	return group;
}