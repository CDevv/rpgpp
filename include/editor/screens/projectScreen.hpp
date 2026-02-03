#ifndef _RPGPP_SCREENS_PROJECT_SCREEN_H
#define _RPGPP_SCREENS_PROJECT_SCREEN_H

#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/HorizontalWrap.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "editor.hpp"
#include "fileInitVisitor.hpp"
#include "fileSystemService.hpp"
#include "fileTab.hpp"
#include "fileViews/fileView.hpp"
#include "guiScreen.hpp"
#include "projectFile.hpp"
#include "projectFileVisitor.hpp"
#include <memory>
#include <vector>

namespace screens {
class ProjectScreen : public UIScreen {
  private:
	std::unique_ptr<ProjectFileVisitor> fileVisitor;
	std::vector<std::unique_ptr<ProjectFile>> openedFiles;
	tgui::Group::Ptr fileViewGroup;
	std::unique_ptr<FileInitVisitor> fileInitVisitor;
	EngineFileType listedResourcesType;
	FileTab::Ptr fileTabs;
	tgui::GrowVerticalLayout::Ptr resourcesLayout;
	void switchView(int index);
	tgui::HorizontalWrap::Ptr createToolBar();
	tgui::Group::Ptr createResourcesList(tgui::Group::Ptr fileViewGroup);

  public:
	void addFileView(EngineFileType fileType, const std::string &path);
	void addResourceButtons(EngineFileType fileType);
	void initItems(tgui::Group::Ptr layout) override;
	const std::string getNameOfScreen() override { return "Project"; }
};
} // namespace screens
#endif