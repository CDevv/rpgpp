#ifndef _RPGPP_INTERACTABLE_H
#define _RPGPP_INTERACTABLE_H

#include "gamedata.hpp"
#include <raylib.h>
#include <map>
#include <array>
#include <string>
#include <memory>
#include <vector>

/** Enum for interactable types */
enum InteractableType {
    INT_BLANK,
    INT_TWO,
    INT_WARPER
};

#define INTTYPE_MAX (3)

/** Defines an object that is interactable in-game by a player's action */
class Interactable {
private:
    /** Whether this Interactable is valid */
    bool valid;
    /** The type of this Interactable */
    InteractableType type;
    /** The position of this Interactable in terms of tiles */
    Vector2 tilePos;
    /** Tile size in pixels */
    int tileSize;
    /** The absolute position of this Interactable in the world scene */
    Vector2 absolutePos;
    /** The Rectangle of this Interactable */
    Rectangle rect;
protected:
    /** Interactable properties. */
    std::map<std::string, std::string> props;
    /** Array for descriptive names of interactable types */
    static std::array<std::string, INTTYPE_MAX> interactableTypeNames;
public:
    /** Empty constructor */
    Interactable();
    /** Construct from the type, tile position and size */
    Interactable(InteractableType type, Vector2 tilePos, int tileSize);
    /** Get the array containing names of the Interactable names */
    static std::array<std::string, INTTYPE_MAX>& getTypeNames();
    /** Whether this Interactable is valid */
    bool isValid() const;
    virtual ~Interactable();
    /** Get the Rectangle of this Interactable */
    Rectangle getRect() const;
    /** Get this Interactable's type */
    InteractableType getType() const;
    /** Get the World position of this Interactable */
    Vector2 getWorldPos() const;
    /** Invoke the virtual interact method. Each Interactable has its own interact method.
     * Shall be invoked when the player interacts with the Interactable */
    virtual void interact();
    /** Set a property of this interactable. */
    void setProp(std::string key, std::string value);
};

struct DiagInt {
    std::string dialogueSource;
};

struct WarperInt {
    std::string targetRoom;
};

class IntBaseWrapper {
public:
    InteractableType type;
    Vector2 pos;
    bool onTouch = false;
    IntBaseWrapper();
    virtual ~IntBaseWrapper();
    virtual void interact();
    void setOnTouch(bool val) { onTouch = val; };
};

/** Templated class for Interactable types */
template<typename T>
class IntBase : public IntBaseWrapper {
private:
    /** The data */
    T data;
public:
    /** Constructor, which takes the position and the type enum. */
    IntBase<T>(Vector2 pos, InteractableType type) { this->pos = pos; this->type = type; };
    /** Constructor, which takes the data itself. */
    IntBase<T>(T value) { data = value; };
    /** Data setter. */
    void set(T data) { this->data = data; };
    /** Data getter. */
    T get() { return data; };
    /** The interact method. */
    void interact();
};

std::unique_ptr<IntBaseWrapper> make_inter_item(Vector2 pos, InteractableType type);
void inter_apply_vec(IntBaseWrapper* inter, std::vector<std::string> props);
void inter_apply_bin(IntBaseWrapper* inter, const InteractableBin &bin);

#endif
