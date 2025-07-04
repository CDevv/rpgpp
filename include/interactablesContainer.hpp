#ifndef _RPGPP_INTERACTABLESCONTAINER_H
#define _RPGPP_INTERACTABLESCONTAINER_H

#include "interactable.hpp"
#include <vector>

class InteractablesContainer {
private:
    std::vector<Interactable> vec;
public:
    InteractablesContainer();
    void add(int x, int y, InteractableType type);
    std::vector<Interactable> getVector();
};

#endif