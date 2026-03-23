#ifndef _RPGPP_SCRIPTSERVICE_H
#define _RPGPP_SCRIPTSERVICE_H

#include "sol/state.hpp"
#include "sol/state_view.hpp"
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

class ScriptService {
  private:
	sol::state state;

  public:
	ScriptService();
	sol::state &getState();
	void setLua(sol::state_view lua);
	void addToState(nlohmann::json &j);
};

#endif