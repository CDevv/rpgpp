#ifndef _RPGPP_PROJECT_H
#define _RPGPP_PROJECT_H

#include <map>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <vector>

#include "gamedata.hpp"
#include "services/fileSystemService.hpp"

using json = nlohmann::json;

class Project {
private:
	std::string projectPath;
	std::string projectTitle;
	IVector windowSize = {640, 480};
	std::string programIconPath = "";
	bool windowResizeableFlag = false;

public:
	Project(const std::string &path);
	static std::string create(const std::string &dirPath, const std::string &title);
	static void openProject(const tgui::String &filePath, bool forceSwitch = false);
	json toJson();
	std::string &getTitle();
	void setTitle(const std::string &newTitle);
	IVector getWindowSize();
	void setWindowSize(IVector newWindowSize);
	std::string &getProgramIconPath();
	void setProgramIconPath(const std::string &newProgramIconPath);
	bool isWindowResizeable();
	void setIsWindowResizeable(bool value);
	std::string &getBasePath();
	std::vector<std::string> getPaths(EngineFileType fileType);
	std::string getResourcePath(EngineFileType fileType, const std::string &fileName);
	std::map<std::string, std::string> getInteractableNames();
	std::vector<std::string> getPropsNames();
	GameData generateStruct();
	void runProject();
	void buildProject();
};

#endif
