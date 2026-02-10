#ifndef _RPGPP_INTERACTABLESCONTAINER_H
#define _RPGPP_INTERACTABLESCONTAINER_H

#include "gamedata.hpp"
#include "interactable.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>
using json = nlohmann::json;

/** Container of Interactables that is to be used by a [Room](Room.md) */
class InteractablesContainer {
  private:
	/** The vector, containing all Interactables */
	std::vector<std::unique_ptr<Interactable>> vec;
	bool interactableExists(int x, int y);

  public:
	/** Empty constructor */
	InteractablesContainer();
	/** Add a new Interactable with tile position and type */
	void add(int x, int y, const std::string &type);
	/** Add a new Interactable using a bin structure */
	void addBin(InteractableInRoomBin bin);
	/** Get an Interactable by its tile position */
	Interactable *getInt(int x, int y) const;
	/** Remove an Interactable by its tile position */
	void removeInteractable(int x, int y);
	/** Change the Interactable's type at the specified tile position */
	void setInteractableType(int x, int y, const std::string &type);
	/** Get the vector that contains all Interactables */
	std::vector<Interactable *> getList() const;
	/** Add interactables from binary structures. */
	void addBinVector(const std::vector<InteractableInRoomBin> &bin);
	/** Add interactables from a Room json object. Must contain 'interactables'
	 * and 'interactables_props' keys. */
	void addJsonData(json roomJson);
	json dumpJson();
};

#endif
