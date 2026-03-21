#include "interactablesContainer.hpp"
#include "conversion.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "tilemap.hpp"
#include <cstdio>
#include <memory>
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <utility>
#include <vector>

InteractablesContainer::InteractablesContainer() {}

Interactable *InteractablesContainer::add(IVector pos,
										  const std::string &type) {
	if (this->objectExistsAtPosition(pos)) {
		return nullptr;
	}

	printf("%s \n", type.c_str());

	Vector2 tilePos = fromIVector(pos);
	this->pushObject(pos, std::move(std::make_unique<Interactable>(
							  type, tilePos, _RPGPP_TILESIZE)));

	return this->getObjectAtPosition(pos).get();
}

void InteractablesContainer::addBin(InteractableInRoomBin bin) {
	IVector pos = {bin.x, bin.y};
	if (this->objectExistsAtPosition(pos)) {
		return;
	}

	this->pushObject(pos, std::move(std::make_unique<Interactable>(bin)));
}

Interactable *InteractablesContainer::getInt(IVector pos) {
	if (!this->objectExistsAtPosition(pos))
		return nullptr;
	return this->getObjectAtPosition(pos).get();
}

void InteractablesContainer::setInteractableType(IVector pos,
												 const std::string &type) {
	auto &obj = this->getObjectAtPosition(pos);

	if (obj->getType() == type) {
		return;
	}

	obj->setType(type);
}

std::vector<Interactable *> InteractablesContainer::getList() {
	std::vector<Interactable *> result;
	for (auto &[vect, interactb] : this->getObjects()) {
		result.push_back(interactb.get());
	}
	return result;
}

void InteractablesContainer::addBinVector(
	const std::vector<InteractableInRoomBin> &bin) {
	for (auto intBin : bin) {
		addBin(intBin);
	}
}

void InteractablesContainer::addJsonData(json roomJson) {
	std::map<std::string, nlohmann::basic_json<>> map = roomJson;

	for (auto const [key, inter] : map) {
		int count = 0;
		char **textSplit = TextSplit(key.c_str(), ';', &count);
		if (count != 2)
			return;
		int x = std::stoi(std::string(textSplit[0]));
		int y = std::stoi(std::string(textSplit[1]));

		std::string src = inter.at("src");
		auto props = inter.at("props");

		auto newInter = add({x, y}, src);
		newInter->setProps(props);
		newInter->setOnTouch(inter.at("onTouch"));
	}
}

json InteractablesContainer::dumpJson() {
	json j = json::object();
	for (auto &[vect, interactible] : this->getObjects()) {
		int tileX = static_cast<int>(interactible->getWorldPos().x);
		int tileY = static_cast<int>(interactible->getWorldPos().y);

		auto key = TextFormat("%i;%i", tileX, tileY);

		auto interProps = interactible->getProps();

		json interJson = json::object();
		interJson.push_back({"src", interactible->getType()});
		interJson.push_back({"props", interProps});
		interJson.push_back({"onTouch", interactible->isOnTouch()});

		j.push_back({key, interJson});
	}

	return j;
}
