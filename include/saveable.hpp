#ifndef _RPGPP_SAVEABLE_H
#define _RPGPP_SAVEABLE_H

#include <nlohmann/json_fwd.hpp>

class ISaveable {
  public:
	ISaveable() = default;
	virtual ~ISaveable() = default;
	virtual nlohmann::json dumpJson() = 0;
};

#endif