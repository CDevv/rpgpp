#include "interactable.hpp"
#include "actor.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "interfaceService.hpp"
#include "sol/forward.hpp"
#include "sol/state.hpp"
#include "sol/state_handling.hpp"
#include "sol/types.hpp"
#include "tilemap.hpp"
#include <exception>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <raylib.h>
#include <stdio.h>
#include <string>

Interactable::Interactable()
	: type(), tilePos(), tileSize(0), absolutePos(), rect() {
	this->valid = false;
}

Interactable::Interactable(const std::string &path) {
	char *jsonString = LoadFileText(path.c_str());
	json intJson = json::parse(jsonString);
	UnloadFileText(jsonString);

	type = GetFileNameWithoutExt(path.c_str());
	displayTitle = intJson.at("name");
	props = std::make_unique<nlohmann::json>(intJson.at("props"));
	scriptPath = intJson.at("script");
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

bool Interactable::isValid() const { return this->valid; }

Rectangle Interactable::getRect() const { return this->rect; }

Vector2 Interactable::getWorldPos() const { return this->tilePos; }

bool Interactable::isOnTouch() const { return onTouch; }

void Interactable::setOnTouch(bool onTouch) { this->onTouch = onTouch; }

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
	auto &state = Game::getScripts().getState();

	Game::getScripts().addToState(*props);

	auto intBin = Game::getBin().interactables.at(type);
	if (Game::getBin().scripts.count(intBin.scriptPath) != 0) {
		auto bc = Game::getBin().scripts[intBin.scriptPath].bytecode;
		auto result = state.safe_script(bc, &sol::script_pass_on_error);
		// auto unsafe_result = state.unsafe_script(bc);

		if (!result.valid()) {
			sol::error error = result;
			std::cout << error.what() << std::endl;
		}
		if (result.status() != sol::call_status::ok) {
			printf("uh oh: %i \n", result.status());
		}

		if (state["interact"].valid()) {
			sol::protected_function f(state["interact"]);
			auto func_result = f();
			if (!func_result.valid()) {
				sol::error error = func_result;
				std::cout << error.what() << std::endl;
			}
		}
	}
}
