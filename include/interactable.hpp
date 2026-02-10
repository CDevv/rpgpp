#ifndef _RPGPP_INTERACTABLE_H
#define _RPGPP_INTERACTABLE_H

#include "gamedata.hpp"
#include "saveable.hpp"
#include <array>
#include <map>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <raylib.h>
#include <string>
#include <vector>

/** Enum for interactable types */
enum InteractableType { INT_BLANK, INT_TWO, INT_WARPER };

#define INTTYPE_MAX (3)

/** Defines an object that is interactable in-game by a player's action */
class Interactable : public ISaveable {
  private:
	/** Whether this Interactable is valid */
	bool valid;
	/** The type of this Interactable */
	std::string type;
	/** The position of this Interactable in terms of tiles */
	Vector2 tilePos;
	/** Tile size in pixels */
	int tileSize;
	/** The absolute position of this Interactable in the world scene */
	Vector2 absolutePos;
	/** The Rectangle of this Interactable */
	Rectangle rect;
	/** Whether this interactable will be interacted with on touch. */
	bool onTouch;
	/** A json object describing the properties of an interactable. */
	std::unique_ptr<nlohmann::json> props;
	/** Script file path */
	std::string scriptPath;

  protected:
	/** Array for descriptive names of interactable types */
	static std::array<std::string, INTTYPE_MAX> interactableTypeNames;

  public:
	/** Empty constructor */
	Interactable();
	/** COnstructor using a JSON file. */
	Interactable(const std::string &path);
	/** Construct from type name */
	Interactable(const std::string &type, Vector2 tilePos, int tileSize);
	/** Construct from a bin */
	Interactable(InteractableInRoomBin bin);
	virtual ~Interactable() = default;
	/** Dump JSON data. */
	nlohmann::json dumpJson();
	/** Get the array containing names of the Interactable names */
	static std::array<std::string, INTTYPE_MAX> &getTypeNames();
	/** Whether this Interactable is valid */
	bool isValid() const;
	/** Get the Rectangle of this Interactable */
	Rectangle getRect() const;
	/** Get this Interactable's type */
	const std::string &getType() const;
	/** Get the World position of this Interactable */
	Vector2 getWorldPos() const;
	/** Get the onTouch member. */
	bool isOnTouch() const;
	/** Invoke the virtual interact method. Each Interactable has its own
	 * interact method. Shall be invoked when the player interacts with the
	 * Interactable */
	virtual void interact();
	/** Change the interactable's type */
	void setType(const std::string &type);
	/** Set a property of this interactable. */
	void setProp(std::string key, std::string value);
	void setProps(nlohmann::json j);
	/** Get source script file. */
	const std::string &getScriptSourcePath();
	/** Get the properties json object. */
	nlohmann::json &getProps();
};

// struct DiagInt {
// 	std::string dialogueSource;
// };

// struct WarperInt {
// 	std::string targetRoom;
// };

// class IntBaseWrapper {
//   public:
// 	InteractableType type;
// 	Vector2 pos;
// 	bool onTouch = false;
// 	IntBaseWrapper();
// 	virtual ~IntBaseWrapper();
// 	virtual void interact();
// 	void setOnTouch(bool val) { onTouch = val; };
// };

// /** Templated class for Interactable types */
// template <typename T> class IntBase : public IntBaseWrapper {
//   private:
// 	/** The data */
// 	T data;

//   public:
// 	/** Constructor, which takes the position and the type enum. */
// 	IntBase<T>(Vector2 pos, InteractableType type) {
// 		this->pos = pos;
// 		this->type = type;
// 	};
// 	/** Constructor, which takes the data itself. */
// 	IntBase<T>(T value) { data = value; };
// 	/** Data setter. */
// 	void set(T data) { this->data = data; };
// 	/** Data getter. */
// 	T get() { return data; };
// 	/** The interact method. */
// 	void interact();
// };

#endif
