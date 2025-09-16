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

void InteractablesContainer::addBinVector(std::vector<InteractableBin> bin)
{
    for (auto intBin : bin) {
        InteractableType itype = static_cast<InteractableType>(intBin.type);
        this->add(intBin.x, intBin.y, itype);

        InteractableTwo* diag;
        switch (itype) {
        case INT_TWO:
            diag = static_cast<InteractableTwo*>(this->get(intBin.x, intBin.y));
            diag->setDialogue(intBin.dialogue);
            break;
        default:
            break;
        }
    }
}

void InteractablesContainer::addJsonData(json roomJson)
{
    if (!roomJson.contains("interactables")) return;

    std::vector<std::vector<int>> interactablesVec = roomJson.at("interactables");
    for (auto v : interactablesVec) {
        int x = v[0];
        int y = v[1];
        InteractableType itype = static_cast<InteractableType>(v[2]);

        this->add(x, y, itype);
    }

    if (roomJson.contains("interactable_props")) {
        std::map<std::string, std::vector<std::string>> interactablesPropsVec = roomJson.at("interactable_props");
        for (auto [key, value] : interactablesPropsVec) {
            int count = 0;
            char** textSplit = TextSplit(key.c_str(), ';', &count);
            if (count != 2) return;

            int x = std::stoi(std::string(textSplit[0]));
            int y = std::stoi(std::string(textSplit[1]));

            Interactable* inter = this->get(x, y);
            if (inter->getType() == INT_TWO) {
                Dialogue dialogue;
                dialogue.lines.push_back(DialogueLine {
                    "Character", value.at(0)
                });

                (static_cast<InteractableTwo*>(inter))->setDialogue(dialogue);
            }
        }
    }
}