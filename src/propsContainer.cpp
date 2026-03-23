#include "propsContainer.hpp"
#include "conversion.hpp"
#include "game.hpp"
#include "prop.hpp"

void PropsContainer::addProp(Vector2 pos, const std::string &type) {
	for (auto const &propBin : Game::getBin().props) {
		if (propBin.name == type) {
			printf("c \n");
			auto p = std::make_unique<Prop>(propBin);
			p->setWorldTilePos(pos,
							   Game::getWorld().getRoom().getWorldTileSize());
			p->getInteractable()->setProps(nlohmann::json::object());

			pushObject(fromVector2(pos), std::move(p));
			break;
		}
	}
}

Prop *PropsContainer::getPropAt(Vector2 pos) {
	return objects[fromVector2(pos)].get();
}