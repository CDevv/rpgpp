#ifndef _RPGPP_INTERACTABLESCONTAINER_H
#define _RPGPP_INTERACTABLESCONTAINER_H

#include "interactable.hpp"
#include "gamedata.hpp"
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/** Container of Interactables that is to be used by a [Room](Room.md) */
class InteractablesContainer {
private:
    /** The vector, containing all Interactables */
    std::vector<std::unique_ptr<Interactable>> vec;
    std::vector<std::unique_ptr<IntBaseWrapper>> test;
public:
    /** Empty constructor */
    InteractablesContainer();
    /** Add a new Interactable with tile position and type */
    void add(int x, int y, InteractableType type);
    /** Get an Interactable by its tile position */
    IntBaseWrapper* getInt(int x, int y) const;
    /** Remove an Interactable by its tile position */
    void removeInteractable(int x, int y);
    /** Change the Interactable's type at the specified tile position */
    void setInteractableType(int x, int y, InteractableType type);
    /** Get the vector that contains all Interactables */
    std::vector<IntBaseWrapper*> getList() const;
    /** Add interactables from binary structures. */
    void addBinVector(const std::vector<InteractableBin> &bin);
    /** Add interactables from a Room json object. Must contain 'interactables'
     * and 'interactables_props' keys. */
    void addJsonData(json roomJson);

    template <typename T>
    void add(int x, int y, InteractableType type);
};

#endif
