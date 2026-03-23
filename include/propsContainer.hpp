#ifndef _RPGPP_PROPSCONTAINER_H
#define _RPGPP_PROPSCONTAINER_H

#include "baseContainer.hpp"
#include "prop.hpp"
#include <memory>

class PropsContainer : public BaseContainer<std::unique_ptr<Prop>> {
  public:
	PropsContainer() = default;
	void addProp(Vector2 pos, const std::string &type);
	Prop *getPropAt(Vector2 pos);
};

#endif