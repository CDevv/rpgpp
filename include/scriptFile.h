
#ifndef RPGPP_SCRIPTFILE_H
#define RPGPP_SCRIPTFILE_H

#include "saveable.hpp"

#include <string>

class ScriptFile: public ISaveable {
	std::string fileContents;

	public:
		ScriptFile() = default;
		ScriptFile(const std::string& path);
		nlohmann::json dumpJson() override;
		std::string& getFileContents();
};

#endif // RPGPP_SCRIPTFILE_H
