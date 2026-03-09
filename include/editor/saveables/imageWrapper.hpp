#ifndef _RPGPP_SAVEABLES_IMAGEWRAPPER_H
#define _RPGPP_SAVEABLES_IMAGEWRAPPER_H

#include "saveable.hpp"
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <raylib.h>
#include <string>

class ImageWrapper : public ISaveable {
  public:
	Image image{};
	std::string source;

	ImageWrapper() = default;
	~ImageWrapper();
	ImageWrapper(const std::string &filePath);
	nlohmann::json dumpJson() { return {}; }
};

#endif