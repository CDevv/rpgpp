#ifndef _RPGPP_BASECONTAINER_H
#define _RPGPP_BASECONTAINER_H

#include "gamedata.hpp"
#include <map>
#include <stdexcept>

template <typename T> class BaseContainer {

  protected:
	/** The list of objects kept inside of this container. */
	std::map<IVector, T> objects = {};

  public:
	BaseContainer<T>() {}

	/** Pushes an object to the map. */
	void pushObject(IVector pos, T obj) {
		this->objects.try_emplace(pos, std::move(obj));
	}
	/** Remove an object existing at a position. */
	void removeObject(IVector pos) {
		auto key = this->objects.find(pos);
		if (key != this->objects.end()) {
			this->objects.erase(key);
		}
	}
	/** Check if an object exists at specified position. */
	bool objectExistsAtPosition(IVector pos) {
		return this->objects.find(pos) != this->objects.end();
	}
	/** Gets the object at a specified IVector Position. */
	T &getObjectAtPosition(IVector pos) {
		if (this->objects.find(pos) != this->objects.end())
			return this->objects[pos];
		throw std::out_of_range("key not found!");
	}
	/** Get a reference to the whole object map. */
	std::map<IVector, T> &getObjects() { return this->objects; }
};

#endif // !_RPGPP_BASECONTAINER_H
