#include "room.hpp"
#include "actor.hpp"
#include "actorContainer.hpp"
#include "collisionsContainer.hpp"
#include "conversion.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "interactablesContainer.hpp"
#include "prop.hpp"
#include "propsContainer.hpp"
#include "tilemap.hpp"
#include <cstdio>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <raylib.h>
#include <utility>
#include <vector>

using json = nlohmann::json;

Room::Room() {
	this->lock = false;

	Camera2D initialCamera;
	initialCamera.offset = Vector2{0, 0};
	initialCamera.target = Vector2{0, 0};
	initialCamera.rotation = 0.0f;
	initialCamera.zoom = 1.0f;
	this->camera = initialCamera;

	this->worldTileSize = 48;
	this->musicSource = "";
	this->interactables = std::make_unique<InteractablesContainer>();
	this->collisions = std::make_unique<CollisionsContainer>();
	this->props = std::make_unique<PropsContainer>();
	this->tileMap = std::unique_ptr<TileMap>{};
	this->actors = std::make_unique<ActorContainer>();
	this->player = std::unique_ptr<Player>{};
}

Room::Room(const std::string &fileName, int tileSize) {
	this->lock = false;

	Camera2D initialCamera;
	initialCamera.offset = Vector2{0, 0};
	initialCamera.target = Vector2{0, 0};
	initialCamera.rotation = 0.0f;
	initialCamera.zoom = 1.0f;
	this->camera = initialCamera;

	this->musicSource = "";
	this->worldTileSize = tileSize;

	char *jsonString = LoadFileText(fileName.c_str());
	json roomJson = json::parse(jsonString);
	UnloadFileText(jsonString);

	std::vector<int> startPosVec = roomJson.at("start_pos");
	startTile = Vector2{static_cast<float>(startPosVec[0]),
						static_cast<float>(startPosVec[1])};

	this->interactables = std::make_unique<InteractablesContainer>();
	this->collisions = std::make_unique<CollisionsContainer>();
	this->actors = std::make_unique<ActorContainer>();
	this->props = std::make_unique<PropsContainer>();

	this->tileMap = std::make_unique<TileMap>(fileName);
	auto actor = std::make_unique<Actor>(DEFAULT_PLAYER_PATH);
	actor->setTilePosition(Vector2{startTile.x, startTile.y},
						   tileMap->getTileSet()->getTileSize());
	auto initialPlayer = std::make_unique<Player>(std::move(actor), *this);

	this->addPlayer(std::move(initialPlayer));

	std::vector<std::vector<int>> collisionsVec = roomJson.at("collision");
	for (auto v : collisionsVec) {
		int x = v[0];
		int y = v[1];

		collisions->pushObject({x, y}, false);
	}

	std::map<std::string, nlohmann::basic_json<>> propsVec =
		roomJson.at("props");
	for (auto const &[key, value] : propsVec) {
		int count = 0;
		char **textSplit = TextSplit(key.c_str(), ';', &count);
		if (count != 2)
			return;
		int x = std::stoi(std::string(textSplit[0]));
		int y = std::stoi(std::string(textSplit[1]));

		auto propVec = value;
		auto p = std::make_unique<Prop>(propVec.at("src"));
		p->setWorldTilePos(
			Vector2{static_cast<float>(x), static_cast<float>(y)},
			worldTileSize);

		p->getInteractable()->setProps(propVec.at("props"));

		props->pushObject({x, y}, std::move(p));
	}

	std::map<std::string, std::string> actorsVec = roomJson.at("actors");
	for (auto const &[key, value] : actorsVec) {
		int count = 0;
		char **textSplit = TextSplit(key.c_str(), ';', &count);
		if (count != 2)
			return;
		int x = std::stoi(std::string(textSplit[0]));
		int y = std::stoi(std::string(textSplit[1]));

		auto a = Actor(value);
		a.setTilePosition(Vector2{static_cast<float>(x), static_cast<float>(y)},
						  Vector2{static_cast<float>(worldTileSize),
								  static_cast<float>(worldTileSize)});
		actors->pushObject({x, y}, std::move(a));
	}

	interactables->addJsonData(roomJson.at("interactables"));

	musicSource = roomJson.at("music_source");
}

Room::Room(std::unique_ptr<TileMap> tileMap) : Room() {
	this->tileMap = std::move(tileMap);
}

Room::Room(const RoomBin &bin) : Room() {
	this->worldTileSize = 48;

	this->interactables = std::make_unique<InteractablesContainer>();
	this->collisions = std::make_unique<CollisionsContainer>();
	this->actors = std::make_unique<ActorContainer>();
	this->props = std::make_unique<PropsContainer>();

	this->tileMap = std::make_unique<TileMap>(bin);

	auto &actorBin = Game::getBin().actors["playerActor"];

	auto actor = std::make_unique<Actor>(actorBin);
	actor->setTilePosition(Vector2{static_cast<float>(bin.startPoint.x),
								   static_cast<float>(bin.startPoint.y)},
						   tileMap->getTileSet()->getTileSize());
	printf("%s \n", actor->getSourcePath().c_str());
	auto initialPlayer = std::make_unique<Player>(std::move(actor), *this);

	this->addPlayer(std::move(initialPlayer));

	interactables->addBinVector(bin.interactables);

	for (auto collisionBin : bin.collisions) {
		collisions->pushObject(collisionBin, false);
	}

	for (auto const &propSource : bin.props) {
		for (auto const &propBin : Game::getBin().props) {
			std::string actualSource =
				GetFileNameWithoutExt(propSource.name.c_str());
			printf("%s ; %s \n", propBin.name.c_str(), actualSource.c_str());
			if (propBin.name == actualSource) {
				printf("c \n");
				auto p = std::make_unique<Prop>(propBin);
				p->setWorldTilePos(
					Vector2{static_cast<float>(propSource.tilePos.x),
							static_cast<float>(propSource.tilePos.y)},
					worldTileSize);
				p->getInteractable()->setProps(
					nlohmann::json::from_cbor(propSource.propsCbor));

				// addProp(std::move(*p));

				props->pushObject(propSource.tilePos, std::move(p));
				break;
			}
		}
	}

	for (const auto &actorSource : bin.actors) {
		for (const auto [name, actorBin] : Game::getBin().actors) {
			if (actorBin.name == actorSource.name) {
				auto a = Actor(actorBin);
				a.setTilePosition(
					Vector2{static_cast<float>(actorSource.tilePos.x),
							static_cast<float>(actorSource.tilePos.y)},
					Vector2{static_cast<float>(worldTileSize),
							static_cast<float>(worldTileSize)});
				actors->pushObject(actorSource.tilePos, std::move(a));
				break;
			}
		}
	}

	musicSource = bin.musicSource;

	if (!bin.musicSource.empty()) {
		Game::getSounds().loadMusic(bin.musicSource);
		Game::getSounds().playMusic();
	} else {
		Game::getSounds().unload();
	}
}

json Room::dumpJson() {
	json roomJson = this->tileMap->dumpJson();

	// Vector for collisions
	auto collisionsVector = std::vector<std::vector<int>>();
	for (auto &[vect, value] : collisions->getObjects()) {
		std::vector<int> collision;
		collision.push_back(static_cast<int>(vect.x));
		collision.push_back(static_cast<int>(vect.y));

		collisionsVector.push_back(collision);
	}
	roomJson.push_back({"collision", collisionsVector});

	// Vector for interactables
	auto interactableProps = interactables->dumpJson();

	auto propsMap = std::map<std::string, nlohmann::json>{};

	for (auto &[pos, prop] : props->getObjects()) {
		std::string key =
			TextFormat("%i;%i", static_cast<int>(prop->getWorldTilePos().x),
					   static_cast<int>(prop->getWorldTilePos().y));

		auto propJson = json::object();
		propJson["src"] =
			TextFormat("props/%s", GetFileName(prop->getSourcePath().c_str()));

		if (prop->getHasInteractable()) {
			propJson["props"] = prop->getInteractable()->getProps();
		} else {
			propJson["props"] = json::object();
		}

		propsMap[key] = propJson;
	}

	auto actorsMap = std::map<std::string, std::string>{};
	for (auto &[vect, obj] : actors->getObjects()) {
		std::string key =
			TextFormat("%i;%i", static_cast<int>(obj.getTilePosition().x),
					   static_cast<int>(obj.getTilePosition().y));

		actorsMap[key] = obj.getSourcePath();
	}

	roomJson.push_back({"interactables", interactableProps});
	roomJson.push_back({"props", propsMap});
	roomJson.push_back({"actors", actorsMap});
	roomJson.push_back({"music_source", musicSource});
	roomJson.push_back(
		{"start_pos",
		 {static_cast<int>(startTile.x), static_cast<int>(startTile.y)}});

	return roomJson;
}

void Room::unload() const {
	tileMap->unload();

	for (auto &[vect, actor] : actors->getObjects()) {
		actor.unload();
	}

	player->unload();
}

void Room::update() {
	for (auto &[vect, actor] : actors->getObjects()) {
		actor.update();
	}
	player->update();
	if (!lock)
		updateCamera();
}

void Room::updateCamera() {
	Vector2 playerPos = player->getCenterPosition();
	Vector2 cameraOffset = {0, 0};
	Vector2 cameraTarget = {0, 0};

	if (playerPos.x >= 320.0f) {
		cameraOffset.x = 320.0f;
		cameraTarget.x = playerPos.x;
	}
	if (playerPos.y >= 240.0f) {
		cameraOffset.y = 240.0f;
		cameraTarget.y = playerPos.y;
	}
	camera.target = cameraTarget;
	camera.offset = cameraOffset;
}

void Room::draw() const {
	BeginMode2D(camera);

	this->tileMap->draw();
	for (auto i : interactables->getList()) {
		auto rect = Rectangle{
			i->getWorldPos().x * static_cast<float>(getWorldTileSize()),
			i->getWorldPos().y * static_cast<float>(getWorldTileSize()),
			static_cast<float>(getWorldTileSize()),
			static_cast<float>(getWorldTileSize())};
		DrawRectangleRec(rect, Fade(YELLOW, 0.5f));
	}
	for (auto &[vect, value] : collisions->getObjects()) {
		auto rect = Rectangle{vect.x * static_cast<float>(worldTileSize),
							  vect.y * static_cast<float>(worldTileSize),
							  static_cast<float>(worldTileSize),
							  static_cast<float>(worldTileSize)};
		DrawRectangleRec(rect, Fade(RED, 0.5f));
	}

	for (auto &[pos, prop] : props->getObjects()) {
		if (prop->getCollisionCenter().y <= player->getCollisionPos().y) {
			prop->draw();
		}
	}

	for (auto &[vect, actor] : actors->getObjects()) {
		actor.draw();
	}
	player->draw();

	for (auto &[pos, prop] : props->getObjects()) {
		if (prop->getCollisionCenter().y > player->getCollisionPos().y) {
			prop->draw();
		}
	}

	EndMode2D();
}

void Room::setWorldTileSize(int tileSize) { this->worldTileSize = tileSize; }

int Room::getWorldTileSize() const { return worldTileSize; }

void Room::setLock(bool val) { this->lock = val; }

void Room::addPlayer(std::unique_ptr<Player> newPlayer) {
	this->player = std::move(newPlayer);
}

Player &Room::getPlayer() const { return *player; }

TileMap *Room::getTileMap() const { return this->tileMap.get(); }

void Room::setTileMap(TileMap *newTileMap) { tileMap.reset(newTileMap); }

std::string Room::getMusicSource() const { return musicSource; }

void Room::setMusicSource(const std::string_view &newMusicSource) {
	this->musicSource = newMusicSource;
}

Vector2 Room::getStartTile() const { return startTile; }

void Room::setStartTile(Vector2 newStartTile) {
	this->startTile = newStartTile;
}

CollisionsContainer &Room::getCollisions() const { return *this->collisions; }

InteractablesContainer &Room::getInteractables() const {
	return *this->interactables;
}

PropsContainer &Room::getProps() const { return *this->props; }

ActorContainer &Room::getActors() const { return *this->actors; }
