#ifndef _RPGPP_SCREENS_PROJECT_SCREEN_H
#define _RPGPP_SCREENS_PROJECT_SCREEN_H

#include "TGUI/Widgets/ContextMenu.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalWrap.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "editor.hpp"
#include "fileInitVisitor.hpp"
#include "fileSystemService.hpp"
#include "fileViews/fileView.hpp"
#include "guiScreen.hpp"
#include "projectFile.hpp"
#include "projectFileVisitor.hpp"
#include "widgets/fileTab.hpp"
#include <memory>
#include <vector>

namespace screens {
class ProjectScreen : public UIScreen {
  private:
    static const int TOOLBAR_H = 54;

    static const int FILETABS_H = 24;

    static const int RESLIST_W = 264;
    static const int RESLIST_RES_CHOOSE_H = 32;
    static const int RESLIST_CREATE_RES_BTN_H = 24;
    static const int RESLIST_RES_BTN_H = 36;
    static const int RESLIST_ITEM_PADDING = 4;

	std::unique_ptr<ProjectFileVisitor> fileVisitor;
	std::vector<std::unique_ptr<ProjectFile>> openedFiles;
	tgui::Group::Ptr fileViewGroup;
	std::unique_ptr<FileInitVisitor> fileInitVisitor;
	EngineFileType listedResourcesType;
	FileTab::Ptr fileTabs;
	tgui::GrowVerticalLayout::Ptr resourcesLayout;
	tgui::ContextMenu::Ptr fileContextMenu;
	void switchView(int index);
	tgui::HorizontalWrap::Ptr createToolBar();
	tgui::Group::Ptr createResourcesList(tgui::Group::Ptr fileViewGroup);

  public:
	void addFileView(EngineFileType fileType, const std::string &path);
	void addResourceButtons(EngineFileType fileType);
	ProjectFile &getCurrentFile();
	void initItems(tgui::Group::Ptr layout) override;
	const std::string getNameOfScreen() override { return "Project"; }
};
} // namespace screens
#endif
