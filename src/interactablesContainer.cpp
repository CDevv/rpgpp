#include "interactablesContainer.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "tilemap.hpp"
#include <memory>
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <vector>

InteractablesContainer::InteractablesContainer() {}

bool InteractablesContainer::interactableExists(int x, int y) {
	for (auto &&i : vec) {
		auto worldPos = i->getWorldPos();
		if (worldPos.x == static_cast<int>(x) &&
			worldPos.y == static_cast<int>(y)) {
			return true;
		}
	}
	return false;
}

void InteractablesContainer::add(int x, int y, const std::string &type) {
	if (interactableExists(x, y)) {
		return;
	}

	Vector2 tilePos = {static_cast<float>(x), static_cast<float>(y)};

	vec.push_back(
		std::make_unique<Interactable>(type, tilePos, _RPGPP_TILESIZE));
}

void InteractablesContainer::addBin(InteractableInRoomBin bin) {
	if (interactableExists(bin.x, bin.y)) {
		return;
	}

	vec.push_back(std::make_unique<Interactable>(bin));
}

Interactable *InteractablesContainer::getInt(int x, int y) const {
	Interactable *res = nullptr;
	for (auto &&i : vec) {
		auto worldPos = i->getWorldPos();
		if (worldPos.x == static_cast<int>(x) &&
			worldPos.y == static_cast<int>(y)) {
			res = i.get();
		}
	}

	return res;
}

void InteractablesContainer::removeInteractable(int x, int y) {
	int idx = 0;
	for (auto &&interactable : this->vec) {
		Interactable *i = interactable.get();
		if (i != nullptr) {
			auto worldPos = i->getWorldPos();
			if (worldPos.x == static_cast<int>(x) &&
				worldPos.y == static_cast<int>(y)) {
				vec.erase(vec.begin() + idx);
			}
		}
		idx++;
	}
}

void InteractablesContainer::setInteractableType(int x, int y,
												 const std::string &type) {
	if (getInt(x, y)->getType() == type) {
		return;
	}

	getInt(x, y)->setType(type);
}

std::vector<Interactable *> InteractablesContainer::getList() const {
	std::vector<Interactable *> result;
	for (auto &&in : this->vec) {
		result.push_back(in.get());
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

		add(x, y, src);

		getInt(x, y)->setProps(inter.at("props"));
	}

	// std::vector<std::vector<int>> interactablesVec =
	// 	roomJson.at("interactables");
	// for (auto v : interactablesVec) {
	// 	int x = v[0];
	// 	int y = v[1];
	// 	InteractableType itype = static_cast<InteractableType>(v[2]);
	// 	bool onTouch = static_cast<bool>(v[3]);

	// 	this->add(x, y, itype);
	// 	getInt(x, y)->setOnTouch(onTouch);
	// }

	// if (roomJson.contains("interactable_props")) {
	// 	std::map<std::string, std::vector<std::string>> interactablesPropsVec =
	// 		roomJson.at("interactable_props");
	// 	for (auto [key, value] : interactablesPropsVec) {
	// 		int count = 0;
	// 		char **textSplit = TextSplit(key.c_str(), ';', &count);
	// 		if (count != 2)
	// 			return;

	// 		int x = std::stoi(std::string(textSplit[0]));
	// 		int y = std::stoi(std::string(textSplit[1]));

	// 		IntBaseWrapper *inter = this->getInt(x, y);

	// 		inter_apply_vec(inter, value);
	// 		/*
	// 		if (inter->type == INT_TWO) {
	// 			DiagInt diagInt;
	// 			diagInt.dialogueSource = value.at(0);

	// 			(static_cast<IntBase<DiagInt>*>(inter))->set(diagInt);
	// 		}
	// 		if (inter->type == INT_WARPER) {
	// 			(static_cast<IntBase<WarperInt>*>(inter))->set({value.at(0)});
	// 		}
	// 		 */
	// 	}
	// }
}

json InteractablesContainer::dumpJson() {
	json j = json::object();
	for (auto &&i : vec) {
		auto *inter = i.get();
		int tileX = static_cast<int>(inter->getWorldPos().x);
		int tileY = static_cast<int>(inter->getWorldPos().y);

		auto key = TextFormat("%i:%i", tileX, tileY);

		json interJson;
		interJson.push_back({"src", inter->getType()});
		interJson.push_back({"props", inter->getProps()});

		j.push_back({key, interJson});
	}

	return j;
}
