#include "screens/projectScreen.hpp"
#include "TGUI/Color.hpp"
#include "TGUI/Layout.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/ContextMenu.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalWrap.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/MessageBox.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "TGUI/Widgets/Tabs.hpp"
#include "editor.hpp"
#include "editorGuiService.hpp"
#include "fileInitVisitor.hpp"
#include "fileSystemService.hpp"
#include "gamedata.hpp"
#include "newFileDialog.hpp"
#include "projectFile.hpp"
#include "projectFileVisitor.hpp"
#include "raylib.h"
#include "widgets/fileTab.hpp"
#include <cstdio>
#include <filesystem>
#include <memory>
#include <system_error>
#include <utility>
#include <vector>

void screens::ProjectScreen::initItems(tgui::Group::Ptr layout) {
	auto &ts = Editor::instance->getTranslations();

	if (!Editor::instance->getGui().menuBar.expired()) {
		auto menuBarPtr = Editor::instance->getGui().menuBar.lock();
		std::vector<tgui::String> saveFileHierarchy = {
			ts.getKey("menu.file"), ts.getKey("menu.file.save_file")};
		std::vector<tgui::String> undoHierarchy = {ts.getKey("menu.edit"),
												   ts.getKey("menu.edit.undo")};
		std::vector<tgui::String> redoHierarchy = {ts.getKey("menu.edit"),
												   ts.getKey("menu.edit.redo")};
		menuBarPtr->setMenuItemEnabled(saveFileHierarchy, true);
		menuBarPtr->connectMenuItem(saveFileHierarchy, [this] {
			if (!openedFiles.empty()) {
				int currentFile = fileTabs->getSelectedIndex();
				printf("%i \n", currentFile);
				auto &projectFile = openedFiles.at(currentFile);
				projectFile->saveFile(projectFile->getFilePath());
			}
		});

		menuBarPtr->setMenuItemEnabled(undoHierarchy, true);
		menuBarPtr->connectMenuItem(
			undoHierarchy, [this] { getCurrentFile().getView().undoAction(); });

		menuBarPtr->setMenuItemEnabled(redoHierarchy, true);
		menuBarPtr->connectMenuItem(
			redoHierarchy, [this] { getCurrentFile().getView().redoAction(); });
	}

	fileContextMenu = tgui::ContextMenu::create();
	fileContextMenu->addMenuItem("Copy full path");
	fileContextMenu->addMenuItem("Delete.");
	Editor::instance->getGui().gui->add(fileContextMenu);

	openedFiles = std::vector<std::unique_ptr<ProjectFile>>{};
	fileVisitor = std::make_unique<ProjectFileVisitor>();
	fileInitVisitor = std::make_unique<FileInitVisitor>();
	listedResourcesType = EngineFileType::FILE_MAP;

	auto toolBar = createToolBar();

	auto fileView =
		tgui::Group::create({TextFormat("100%% - %d", RESLIST_W),
							 TextFormat("100%% - %d", TOOLBAR_H + FILETABS_H)});
	fileView->setPosition(RESLIST_W, TOOLBAR_H + FILETABS_H);
	layout->add(fileView);
	this->fileViewGroup = fileView;

	auto label = tgui::Label::create("Test");
	fileView->add(label);

	auto panel = createResourcesList(fileView);

	layout->add(panel);
	layout->add(toolBar);

	auto tabs2 = tgui::Tabs::create();

	fileTabs = FileTab::create();
	fileTabs->setSize(TextFormat("100%% - %d", RESLIST_W), FILETABS_H);
	fileTabs->setPosition(RESLIST_W, TOOLBAR_H);

	fileTabs->onTabClose([this](int i) {
		printf("tab close %i \n", i);
		openedFiles.erase(openedFiles.begin() + i);

		if (fileTabs->getTabsCount() == 0) {
			fileViewGroup->removeAllWidgets();
		} else {
			int selected = fileTabs->getSelectedIndex();
			if (selected >= 0) {
				switchView(selected);
			} else {
				fileViewGroup->removeAllWidgets();
			}
		}
	});
	fileTabs->onTabSelect([this](int i) { switchView(i); });

	layout->add(fileTabs);

	// Maximize when a project is opened
	SetWindowState(FLAG_WINDOW_MAXIMIZED);
}

void screens::ProjectScreen::addFileView(EngineFileType fileType,
										 const std::string &path) {
	size_t idx = fileTabs->addFileTab(GetFileName(path.c_str()));
	if (idx != -1) {
    	fileViewGroup->removeAllWidgets();
    	std::unique_ptr<ProjectFile> projectFile =
    		fileVisitor->visit(fileType, path);
    	projectFile->initUi(fileViewGroup);
    	projectFile->setFilePath(path);
		openedFiles.insert(openedFiles.begin() + idx, std::move(projectFile));
	}
}

void screens::ProjectScreen::switchView(int index) {
	fileViewGroup->removeAllWidgets();
	openedFiles.at(index)->addWidgets(fileViewGroup);
}

tgui::HorizontalWrap::Ptr screens::ProjectScreen::createToolBar() {
	auto toolBar = tgui::HorizontalWrap::create({"100%", TOOLBAR_H});
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
	playBtn->onPress([] { Editor::instance->getProject()->runProject(); });
	toolBar->add(playBtn);

	auto buildBtn = tgui::BitmapButton::create();
	auto buildImg = tgui::Texture(fs.getResourcePath("build.png"));
	buildBtn->setImage(buildImg);
	buildBtn->setSize({barSize, "100%"});
	buildBtn->onPress([project] {
		std::filesystem::path path = project->getBasePath();
		path /= "game.bin";

		auto data = project->generateStruct();
		serializeDataToFile(path, data);
	});
	toolBar->add(buildBtn);

	return toolBar;
}

void screens::ProjectScreen::addResourceButtons(EngineFileType fileType) {
	auto project = Editor::instance->getProject();

	this->listedResourcesType = fileType;

	resourcesLayout->removeAllWidgets();
	resourcesLayout->getRenderer()->setSpaceBetweenWidgets(RESLIST_ITEM_PADDING);

	for (auto filePath : project->getPaths(fileType)) {
		auto fileBtn = tgui::Button::create(GetFileName(filePath.c_str()));
		fileBtn->setSize("100%", RESLIST_RES_BTN_H);
		fileBtn->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0));
		fileBtn->onPress(
			[this, fileType, filePath] { addFileView(fileType, filePath); });
		fileBtn->onRightMousePress([this, filePath] {
			fileContextMenu->getMenuItems().at(0).text = filePath;
			fileContextMenu->setPosition(GetMousePosition().x,
										 GetMousePosition().y);
			fileContextMenu->onMenuItemClick.disconnectAll();
			fileContextMenu->onMenuItemClick(
				[this, filePath](const std::vector<tgui::String> &hierarchy) {
					if (hierarchy[0] == "Copy full path") {
						SetClipboardText(filePath.c_str());
					}
					if (hierarchy[0] == "Delete.") {
						auto messageBox = tgui::MessageBox::create();
						messageBox->setText("Are you sure?");
						messageBox->addButton("Yes");
						messageBox->addButton("No");
						messageBox->setButtonAlignment(
							tgui::HorizontalAlignment::Right);
						EditorGuiService::centerWidget(messageBox);

						messageBox->onButtonPress(
							[this, msgBox = messageBox.get(),
							 filePath](const tgui::String &button) {
								assert(button == "Yes" || button == "No");
								if (button == "Yes") {
									std::error_code ec;
									std::filesystem::remove(filePath, ec);
									addResourceButtons(listedResourcesType);
									if (ec) {
										printf("%s \n", ec.message().c_str());
									}
								}
								msgBox->getParent()->remove(
									msgBox->shared_from_this());
							});

						Editor::instance->getGui().gui->add(messageBox);
					}
				});
			fileContextMenu->openMenu();
		});

		resourcesLayout->add(fileBtn);
	}
}

tgui::Group::Ptr
screens::ProjectScreen::createResourcesList(tgui::Group::Ptr fileViewGroup) {
	auto project = Editor::instance->getProject();
	TranslationService &tService = Editor::instance->getTranslations();

	auto group =
		tgui::Group::create({RESLIST_W, TextFormat("100%% - %d", TOOLBAR_H)});
	group->setPosition(0, TOOLBAR_H);

	auto resourceChoose = tgui::ComboBox::create();
	resourceChoose->setPosition(0, 0);
	resourceChoose->setSize("100%", RESLIST_RES_CHOOSE_H);
	resourceChoose->addMultipleItems({"TileSets", "Maps", "Scripts"});
	resourceChoose->setSelectedItem("Maps");
	group->add(resourceChoose);

	auto createResourceBtn = tgui::Button::create(tService.getKey("screen.project.create_new_resource"));
	createResourceBtn->setPosition(0, RESLIST_RES_CHOOSE_H);
	createResourceBtn->setSize("100%", RESLIST_CREATE_RES_BTN_H);
	createResourceBtn->onPress([this] {
		if (!fileInitVisitor->funcIsEmpty(listedResourcesType)) {
			auto childDialog = NewFileDialog::create();
			childDialog->init(Editor::instance->getGui().gui.get());
			EditorGuiService::centerWidget(childDialog->window);
			fileInitVisitor->visit(listedResourcesType, childDialog);
		}
	});
	group->add(createResourceBtn);

	auto resourceListPanel = tgui::ScrollablePanel::create(
		{"100%", TextFormat("100%% - %d", FILETABS_H + RESLIST_RES_CHOOSE_H +
											  RESLIST_CREATE_RES_BTN_H)});
	resourceListPanel->setPosition(0, RESLIST_RES_CHOOSE_H +
										  RESLIST_CREATE_RES_BTN_H);
	resourceListPanel->getVerticalScrollbar()->setPolicy(
		tgui::Scrollbar::Policy::Automatic);
	resourceListPanel->getHorizontalScrollbar()->setPolicy(
		tgui::Scrollbar::Policy::Never);
	resourceListPanel->getRenderer()->setBackgroundColor(
		tgui::Color::applyOpacity(tgui::Color::Black, 0.2));

	resourcesLayout = tgui::GrowVerticalLayout::create();
	resourceListPanel->add(resourcesLayout);
	group->add(resourceListPanel);

	resourceChoose->onItemSelect([this, &fileViewGroup](int index) {
		EngineFileType currentFileType = static_cast<EngineFileType>(index);
		addResourceButtons(currentFileType);
	});

	if (project != nullptr) {
		EngineFileType currentFileType =
			static_cast<EngineFileType>(resourceChoose->getSelectedItemIndex());
		addResourceButtons(currentFileType);
	}

	return group;
}

ProjectFile &screens::ProjectScreen::getCurrentFile() {
	int currentFile = fileTabs->getSelectedIndex();
	return *openedFiles.at(currentFile);
}
