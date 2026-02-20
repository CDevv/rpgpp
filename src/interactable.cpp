#include "interactable.hpp"
#include "actor.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "interfaceService.hpp"
#include "sol/state.hpp"
#include "sol/types.hpp"
#include "tilemap.hpp"
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <raylib.h>
#include <stdio.h>

std::array<std::string, INTTYPE_MAX> Interactable::interactableTypeNames = {
	"Blank", "Dialogue", "Warper"};

Interactable::Interactable()
	: type(), tilePos(), tileSize(0), absolutePos(), rect() {
	this->valid = false;
}

Interactable::Interactable(const std::string &path) {
	char *jsonString = LoadFileText(path.c_str());
	json intJson = json::parse(jsonString);

	type = GetFileNameWithoutExt(path.c_str());
	displayTitle = intJson.at("name");
	props = std::make_unique<nlohmann::json>(intJson.at("props"));
	scriptPath = intJson.at("script");

	UnloadFileText(jsonString);
}

Interactable::Interactable(const std::string &type, Vector2 tilePos,
						   int tileSize) {
	this->type = type;
	this->props = std::make_unique<nlohmann::json>();

	this->valid = true;
	this->type = type;
	this->tilePos = tilePos;
	this->tileSize = tileSize;
	this->absolutePos = Vector2{0, 0};

	rect =
		Rectangle{tilePos.x * tileSize, tilePos.y * tileSize,
				  static_cast<float>(tileSize), static_cast<float>(tileSize)};
}

Interactable::Interactable(InteractableInRoomBin bin) {
	this->type = bin.type;
	this->props =
		std::make_unique<nlohmann::json>(json::from_cbor(bin.propsCbor));

	Vector2 tilePos = {static_cast<float>(bin.x), static_cast<float>(bin.y)};

	this->valid = true;
	this->tilePos = tilePos;
	this->tileSize = _RPGPP_TILESIZE;
	this->absolutePos = Vector2{0, 0};

	rect =
		Rectangle{tilePos.x * tileSize, tilePos.y * tileSize,
				  static_cast<float>(tileSize), static_cast<float>(tileSize)};
}

json Interactable::dumpJson() {
	json j = json::object();
	j.push_back({"name", displayTitle});
	j.push_back({"props", *props});
	j.push_back({"script", scriptPath});

	return j;
}

std::array<std::string, INTTYPE_MAX> &Interactable::getTypeNames() {
	return interactableTypeNames;
}

bool Interactable::isValid() const { return this->valid; }

Rectangle Interactable::getRect() const { return this->rect; }

Vector2 Interactable::getWorldPos() const { return this->tilePos; }

bool Interactable::isOnTouch() const { return onTouch; }

const std::string &Interactable::getType() const { return this->type; }

void Interactable::setType(const std::string &type) {
	this->type = type;
	this->props = std::make_unique<nlohmann::json>();
}

void Interactable::setProps(nlohmann::json j) {
	this->props = std::make_unique<nlohmann::json>(j);
}

nlohmann::json &Interactable::getProps() { return *props; }

const std::string &Interactable::getScriptSourcePath() { return scriptPath; }

void Interactable::setDisplayTitle(const std::string &newTitle) {
	displayTitle = newTitle;
}

std::string &Interactable::getDisplayTitle() { return displayTitle; }

void Interactable::interact() {
	if (type == "dialogue") {
		printf("interaction!\n");
	} else {
		printf("two.\n");
	}

	sol::state lua;
	lua.open_libraries(sol::lib::base);
	Game::setLua(lua);

	printf("type: %s \n", type.c_str());

	auto intBin = Game::getBin().interactables.at(type);
	if (Game::getBin().scripts.count(intBin.scriptPath) != 0) {
		auto bc = Game::getBin().scripts[intBin.scriptPath].bytecode;
		auto result = lua.script(bc);
		if (!result.valid()) {
			printf("uh oh. \n");
			return;
		}

		if (lua["interact"].valid()) {
			lua["interact"].call<void>();
		}
	}
}
