#ifndef _RPGPP_PROJECTSETTINGSWINDOW_H
#define _RPGPP_PROJECTSETTINGSWINDOW_H

#include <memory>

#include "childWindows/popupWindow.hpp"
#include "childWindows/projectSettingsPanel/game.hpp"
#include "childWindows/projectSettingsPanel/program.hpp"

class ProjectSettingsWindow : public PopupWindow {
private:
	std::unique_ptr<ProjectSettingsPanelProgram> program;
	std::unique_ptr<ProjectSettingsPanelGame> game;

public:
	ProjectSettingsWindow();
	void open() override;
};

#endif