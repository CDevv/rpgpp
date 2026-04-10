#ifndef _RPGPP_SAVEABLES_SOUNDWRAPPER_H
#define _RPGPP_SAVEABLES_SOUNDWRAPPER_H

#include <raylib.h>

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>

#include "saveable.hpp"

class SoundWrapper : public ISaveable {
public:
	Music sound{};
	std::string source;
	SoundWrapper() = default;
	~SoundWrapper();
	SoundWrapper(const std::string &filePath);
	nlohmann::json dumpJson() { return {}; }
};

#endif