#ifndef _RPGPP_STATESERVICE_H
#define _RPGPP_STATESERVICE_H

#include <map>
#include <string>

class StateService {
private:
    std::map<std::string, bool> gameState;
public:
    StateService();
    bool getProp(std::string prop);
    void unload();
};

#endif
