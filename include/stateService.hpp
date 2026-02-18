#ifndef _RPGPP_STATESERVICE_H
#define _RPGPP_STATESERVICE_H

#include <map>
#include <string>

/** The StateService is responsible for storing gameplay-related variables
 * that make up the state of the game. */
class StateService {
  private:
	/** A pair of string keys and boolean values. */
	std::map<std::string, bool> gameState;

  public:
	/** Empty constructor */
	StateService();
	/** Get a boolean property from the container. */
	bool getProp(const std::string &prop) const;
	/** Unload routine. */
	void unload() const;
};

#endif
