#include "interactablesContainer.hpp"
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

Interactable *InteractablesContainer::add(int x, int y,
										  const std::string &type) {
	if (interactableExists(x, y)) {
		return nullptr;
	}

	printf("%s \n", type.c_str());

	Vector2 tilePos = {static_cast<float>(x), static_cast<float>(y)};

	std::unique_ptr<Interactable> inter =
		std::make_unique<Interactable>(type, tilePos, _RPGPP_TILESIZE);

	vec.push_back(std::move(inter));

	return getInt(x, y);
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
		auto props = inter.at("props");

		add(x, y, src);

		getInt(x, y)->setProps(props);
	}
}

json InteractablesContainer::dumpJson() {
	json j = json::object();
	for (auto &&i : vec) {
		auto *inter = i.get();
		int tileX = static_cast<int>(inter->getWorldPos().x);
		int tileY = static_cast<int>(inter->getWorldPos().y);

		auto key = TextFormat("%i;%i", tileX, tileY);

		auto interProps = inter->getProps();

		json interJson = json::object();
		interJson.push_back({"src", inter->getType()});
		interJson.push_back({"props", interProps});

		j.push_back({key, interJson});
	}

	return j;
}
