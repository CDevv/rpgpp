#include "screens/projectScreen.hpp"
#include "TGUI/Color.hpp"
#include "TGUI/Layout.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/ContextMenu.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/MessageBox.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "TGUI/Widgets/Tabs.hpp"
#include "components/resizableContainer.hpp"
#include "editor.hpp"
#include "fileInitVisitor.hpp"
#include "gamedata.hpp"
#include "projectFile.hpp"
#include "projectFileVisitor.hpp"
#include "raylib.h"
#include "services/editorGuiService.hpp"
#include "services/fileSystemService.hpp"
#include "widgets/fileTab.hpp"
#include "widgets/newFileDialog.hpp"
#include <cstdio>
#include <filesystem>
#include <memory>
#include <system_error>
#include <utility>
#include <vector>

void screens::ProjectScreen::layoutReload() {
	resListWBinder->setSize(modifiable_RESLIST_W, "100%");
}

void screens::ProjectScreen::mouseMove(int x, int y) {
	resourcesList->manualMouseMoved(
		{static_cast<float>(x), static_cast<float>(y)});
	fileTabs->manualMouseMoved({static_cast<float>(x), static_cast<float>(y)});
}

void screens::ProjectScreen::leftMouseReleased(int x, int y) {
	resourcesList->manualLeftMouseReleased(
		{static_cast<float>(x), static_cast<float>(y)});
	fileTabs->manualLeftMouseReleased(
		{static_cast<float>(x), static_cast<float>(y)});
}

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
				tgui::String currentFile = fileTabs->getSelectedId();
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

	openedFiles = std::map<tgui::String, std::unique_ptr<ProjectFile>>{};
	fileVisitor = std::make_unique<ProjectFileVisitor>();
	fileInitVisitor = std::make_unique<FileInitVisitor>();
	listedResourcesType = EngineFileType::FILE_MAP;

	resListWBinder = tgui::Group::create({modifiable_RESLIST_W, 0});
	layout->add(resListWBinder, "resListWBinder");

	auto fileView = tgui::Group::create(
		{tgui::Layout("100%") - tgui::bindWidth(resListWBinder),
		 tgui::Layout("100%") - tgui::Layout(TOOLBAR_H + FILETABS_H)});
	fileView->setPosition(tgui::bindWidth(resListWBinder),
						  TOOLBAR_H + FILETABS_H);
	layout->add(fileView);
	this->fileViewGroup = fileView;

	clearView();

	resourcesList = createResourcesList();
	auto toolBar = createToolBar();

	layout->add(resourcesList);
	layout->add(toolBar);

	auto tabs2 = tgui::Tabs::create();

	fileTabs = FileTab::create();
	fileTabs->setSize(tgui::Layout("100%") - tgui::bindWidth(resListWBinder),
					  FILETABS_H);
	fileTabs->setPosition(tgui::bindWidth(resListWBinder), TOOLBAR_H);
	fileTabs->useExternalMouseEvent = true;

	fileTabs->onTabClose([this](tgui::String id) {
		openedFiles.erase(id);
		if (fileTabs->getTabsCount() == 0) {
			clearView();
		}
	});
	fileTabs->onTabSelect([this](tgui::String id) {
		if (fileTabs->getTabsCount() == 0) {
			clearView();
		} else {
			switchView(id);
		}
	});

	layout->add(fileTabs);

	// Maximize when a project is opened
	SetWindowState(FLAG_WINDOW_MAXIMIZED);
}

void screens::ProjectScreen::addFileView(EngineFileType fileType,
										 const std::string &path) {

	Editor::instance->getGui().gui->setTabKeyUsageEnabled(
		fileType != EngineFileType::FILE_SCRIPT);

	size_t idx = fileTabs->addFileTab(path, GetFileName(path.c_str()));
	if (idx != -1) {
		fileViewGroup->removeAllWidgets();
		std::unique_ptr<ProjectFile> projectFile =
			fileVisitor->visit(fileType, path);
		projectFile->initUi(fileViewGroup);
		projectFile->setFilePath(path);
		tgui::String id = path;
		openedFiles.try_emplace(id, std::move(projectFile));
	}
}

void screens::ProjectScreen::switchView(tgui::String id) {
	fileViewGroup->removeAllWidgets();
	openedFiles.at(id)->addWidgets(fileViewGroup);
}

void screens::ProjectScreen::clearView() {
	fileViewGroup->removeAllWidgets();
	std::unique_ptr<ProjectFile> empty =
		fileVisitor->visit(EngineFileType::FILE_EMPTY, ".");
	empty->initUi(fileViewGroup);
	empty->addWidgets(fileViewGroup);
}

tgui::Group::Ptr screens::ProjectScreen::createToolBar() {
	auto toolBar = tgui::Group::create({"100%", TOOLBAR_H});
	toolBar->setPosition(0, 0);
	// toolBar->getRenderer()->setSpaceBetweenWidgets(8.0f);
	toolBar->getRenderer()->setPadding(8);

	auto barSize = toolBar->getSize().y;

	auto project = Editor::instance->getProject();

	projectLabel = tgui::Label::create("text.");
	projectLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
	projectLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	projectLabel->setTextSize(16);
	projectLabel->setPosition({0, 0});
	projectLabel->setSize({tgui::bindWidth(resListWBinder), "100%"});

	if (project == nullptr) {
		projectLabel->setText("uhh, nullptr");
	} else {
		projectLabel->setText(project->getTitle());
	}

	toolBar->add(projectLabel, "projectLabel");

	auto &fs = Editor::instance->getFs();

	auto playBtn = tgui::BitmapButton::create();
	auto playtestImg = tgui::Texture(fs.getResourcePath("playtest.png"));
	playBtn->setImage(playtestImg);
	playBtn->setSize({barSize, "100%"});
	playBtn->setPosition({tgui::bindRight(projectLabel), 0});
	playBtn->onPress([] { Editor::instance->getProject()->runProject(); });
	toolBar->add(playBtn, "playBtn");

	auto buildBtn = tgui::BitmapButton::create();
	auto buildImg = tgui::Texture(fs.getResourcePath("build.png"));
	buildBtn->setImage(buildImg);
	buildBtn->setSize({barSize, "100%"});
	buildBtn->setPosition({tgui::bindRight(playBtn) + 8, 0});
	buildBtn->onPress([project] {
		std::filesystem::path path = project->getBasePath();
		path /= "game.bin";

		auto data = project->generateStruct();
		serializeDataToFile(path.u8string(), data);
	});
	toolBar->add(buildBtn);

	return toolBar;
}

void screens::ProjectScreen::addResourceButtons(EngineFileType fileType) {
	auto project = Editor::instance->getProject();

	this->listedResourcesType = fileType;

	resourcesLayout->removeAllWidgets();
	resourcesLayout->getRenderer()->setSpaceBetweenWidgets(
		RESLIST_ITEM_PADDING);

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

ResizableContainer::Ptr screens::ProjectScreen::createResourcesList() {
	auto project = Editor::instance->getProject();
	TranslationService &tService = Editor::instance->getTranslations();

	auto group = ResizableContainer::create(
		{modifiable_RESLIST_W, tgui::Layout("100%") - TOOLBAR_H},
		{0, TOOLBAR_H});
	group->enableResize(ResizeDirection::RIGHT);
	group->setMinResizeWidth(MIN_RESLIST_W);
	group->setMaxResizeWidth(MAX_RESLIST_W);
	group->useExternalMouseEvent = true;
	group->onResize([this, group] {
		modifiable_RESLIST_W = group->getSize().x;
		layoutReload();
	});

	auto resourceChoose = tgui::ComboBox::create();
	resourceChoose->setPosition(0, 0);
	resourceChoose->setSize("100%", RESLIST_RES_CHOOSE_H);
	resourceChoose->addMultipleItems({"TileSets", "Maps", "Scripts"});
	resourceChoose->setSelectedItem("Maps");
	group->add(resourceChoose);

	auto createResourceBtn = tgui::Button::create(
		tService.getKey("screen.project.create_new_resource"));
	createResourceBtn->setPosition(0, tgui::bindBottom(resourceChoose));
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
		{"100%", tgui::Layout("100%") -
					 (RESLIST_RES_CHOOSE_H + RESLIST_CREATE_RES_BTN_H)});
	resourceListPanel->setPosition(0, tgui::bindBottom(createResourceBtn));
	resourceListPanel->getVerticalScrollbar()->setPolicy(
		tgui::Scrollbar::Policy::Automatic);
	resourceListPanel->getHorizontalScrollbar()->setPolicy(
		tgui::Scrollbar::Policy::Never);
	resourceListPanel->getRenderer()->setBackgroundColor(
		tgui::Color::applyOpacity(tgui::Color::Black, 0.2));

	resourcesLayout = tgui::GrowVerticalLayout::create();
	resourceListPanel->add(resourcesLayout);
	group->add(resourceListPanel);

	resourceChoose->onItemSelect([this](int index) {
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
	tgui::String currentFile = fileTabs->getSelectedId();
	return *openedFiles.at(currentFile);
}
