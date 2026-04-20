#ifndef _RPGPP_SAVEABLES_FONTWRAPPER_H
#define _RPGPP_SAVEABLES_FONTWRAPPER_H

#include <raylib.h>

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>

#include "saveable.hpp"

class FontWrapper : public ISaveable {
public:
	Font font{};
	std::string fontSource;
	FontWrapper() = default;
	~FontWrapper();
	FontWrapper(const std::string &filePath);
	nlohmann::json dumpJson() { return {}; }
};

#endif