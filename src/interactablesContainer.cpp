#include "interactablesContainer.hpp"
#include "interactable.hpp"
#include <memory>
#include <raylib.h>
#include <vector>

template <typename T>
std::unique_ptr<IntBaseWrapper> make_item(T const& value) {
    return std::unique_ptr<IntBaseWrapper>(new IntBase<T>(value));
}

template <typename T>
std::unique_ptr<IntBaseWrapper> make_item(Vector2 pos, InteractableType type) {
    return std::unique_ptr<IntBaseWrapper>(new IntBase<T>(pos, type));
}

InteractablesContainer::InteractablesContainer() {}

void InteractablesContainer::add(int x, int y, InteractableType type) {
    Vector2 position = { static_cast<float>(x), static_cast<float>(y) };

    switch (type) {
        case INT_BLANK:
            add<int>(x, y, type);
            break;
        case INT_TWO:
            add<DiagInt>(x, y, type);
            break;
        case INT_WARPER:
            add<WarperInt>(x, y, type);
            break;
    }
}

template <typename T>
void InteractablesContainer::add(int x, int y, InteractableType type)
{
    test.push_back(make_item<T>(Vector2 {
        static_cast<float>(x), static_cast<float>(y)
    }, type));
}

IntBaseWrapper* InteractablesContainer::getInt(int x, int y)
{
    IntBaseWrapper* res = nullptr;
    for (auto&& i : test) {
        if (i->pos.x == x && i->pos.y == y) {
            res = i.get();
        }
    }

    return res;
}

void InteractablesContainer::removeInteractable(int x, int y)
{
    for (std::vector<std::unique_ptr<IntBaseWrapper>>::iterator it = test.begin(); it != test.end(); ) {
        if (it->get()->pos.x == x && it->get()->pos.y == y) {
            test.erase(it);
        } else {
            ++it;
        }
    }
}

void InteractablesContainer::setInteractableType(int x, int y, InteractableType type)
{
    if (getInt(x, y)->type == type) {
        return;
    }

    this->removeInteractable(x, y);
    this->add(x, y, type);
}

std::vector<IntBaseWrapper*> InteractablesContainer::getList()
{
    std::vector<IntBaseWrapper*> result;
    for (auto&& in : this->test) {
        result.push_back(in.get());
    }
    return result;
}

void InteractablesContainer::addBinVector(std::vector<InteractableBin> bin)
{
    for (auto intBin : bin) {
        InteractableType itype = static_cast<InteractableType>(intBin.type);
        this->add(intBin.x, intBin.y, itype);
        bool onTouch = static_cast<bool>(intBin.onTouch);
        getInt(intBin.x, intBin.y)->setOnTouch(onTouch);

        inter_apply_bin(getInt(intBin.x, intBin.y), intBin);
        /*
        IntBase<DiagInt>* diag;
        switch (itype) {
        case INT_TWO:
            diag = static_cast<IntBase<DiagInt>*>(this->getInt(intBin.x, intBin.y));
            diag->set(DiagInt {intBin.dialogue});
            break;
        case INT_WARPER:
            static_cast<IntBase<WarperInt>*>(this->getInt(intBin.x, intBin.y))->set({
                intBin.dialogue
            });
            break;
        default:
            break;
        }
        */
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
        bool onTouch = static_cast<bool>(v[3]);

        this->add(x, y, itype);
        getInt(x, y)->setOnTouch(onTouch);
    }

    if (roomJson.contains("interactable_props")) {
        std::map<std::string, std::vector<std::string>> interactablesPropsVec = roomJson.at("interactable_props");
        for (auto [key, value] : interactablesPropsVec) {
            int count = 0;
            char** textSplit = TextSplit(key.c_str(), ';', &count);
            if (count != 2) return;

            int x = std::stoi(std::string(textSplit[0]));
            int y = std::stoi(std::string(textSplit[1]));

            IntBaseWrapper* inter = this->getInt(x, y);

            inter_apply_vec(inter, value);
            /*
            if (inter->type == INT_TWO) {
                DiagInt diagInt;
                diagInt.dialogueSource = value.at(0);

                (static_cast<IntBase<DiagInt>*>(inter))->set(diagInt);
            }
            if (inter->type == INT_WARPER) {
                (static_cast<IntBase<WarperInt>*>(inter))->set({value.at(0)});
            }
             */
        }
    }
}
