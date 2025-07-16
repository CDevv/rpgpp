#include "interactablesContainer.hpp"
#include "interactable.hpp"
#include <raylib.h>
#include <vector>

InteractablesContainer::InteractablesContainer() {}

void InteractablesContainer::add(int x, int y, InteractableType type) {
    Vector2 position = { static_cast<float>(x), static_cast<float>(y) };

    switch (type) {
        case INT_BLANK:
            vec.push_back(Interactable(type, position, 48));
            break;
        case INT_TWO:
            vec.push_back(InteractableTwo(position, 48));
            break;
    }
}

void InteractablesContainer::removeInteractable(int x, int y) {
    for (std::vector<Interactable>::iterator it = vec.begin(); it != vec.end();) {
        if (it->getWorldPos().x == x && it->getWorldPos().y == y) {
            vec.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<Interactable> InteractablesContainer::getVector() {
    return this->vec;
}