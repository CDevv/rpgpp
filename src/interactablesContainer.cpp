#include "interactablesContainer.hpp"
#include "interactable.hpp"
#include <raylib.h>
#include <vector>

InteractablesContainer::InteractablesContainer() {}

void InteractablesContainer::add(int x, int y, InteractableType type) {
    Vector2 position = { static_cast<float>(x), static_cast<float>(y) };

    switch (type) {
        case INT_BLANK:
            vec.push_back(std::make_unique<InteractableOne>(position, 48));
            break;
        case INT_TWO:
            vec.push_back(std::make_unique<InteractableTwo>(position, 48));
            break;
    }
}

Interactable* InteractablesContainer::get(int x, int y)
{
    Interactable* result = nullptr;
    for (auto&& i : vec) {
        if (i->getWorldPos().x == x && i->getWorldPos().y == y) {
            result = i.get();
        }
    }

    return result;
}

void InteractablesContainer::removeInteractable(int x, int y) {
    for (std::vector<std::unique_ptr<Interactable>>::iterator it = vec.begin(); it != vec.end();) {
        if (it->get()->getWorldPos().x == x && it->get()->getWorldPos().y == y) {
            vec.erase(it);
        } else {
            ++it;
        }
    }
}

void InteractablesContainer::setInteractableType(int x, int y, InteractableType type)
{
    this->removeInteractable(x, y);
    this->add(x, y, type);
}

std::vector<Interactable*> InteractablesContainer::getVector() 
{
    std::vector<Interactable*> result;
    for (auto&& in : this->vec) {
        result.push_back(in.get());
    }
    return result;
}