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
	ProjectProgramSettings programSet;
	ProjectGameSettings gameSet;

public:
	Project() = default;
	Project(const std::string &path);
	static std::string create(const std::string &dirPath, const std::string &title);
	static void openProject(const tgui::String &filePath, bool forceSwitch = false);
	json toJson();

	ProjectProgramSettings &getProgramSettings();
	ProjectGameSettings &getGameSettings();

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
