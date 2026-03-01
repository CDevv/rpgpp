#ifndef _RPGPP_SAVEABLES_FONTWRAPPER_H
#define _RPGPP_SAVEABLES_FONTWRAPPER_H

#include "saveable.hpp"
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <raylib.h>
#include <string>

class FontWrapper : public ISaveable {
  public:
	Font font{};
	FontWrapper() = default;
	~FontWrapper();
	FontWrapper(const std::string &filePath);
	nlohmann::json dumpJson() { return {}; }
};

#endif