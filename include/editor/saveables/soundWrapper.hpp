#ifndef _RPGPP_SAVEABLES_SOUNDWRAPPER_H
#define _RPGPP_SAVEABLES_SOUNDWRAPPER_H

#include "saveable.hpp"
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <raylib.h>
#include <string>

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