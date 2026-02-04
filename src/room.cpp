#include "room.hpp"
#include "actor.hpp"
#include "collisionsContainer.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "interactablesContainer.hpp"
#include "prop.hpp"
#include "tilemap.hpp"
#include <cstdio>
#include <memory>
#include <nlohmann/json.hpp>
#include <raylib.h>
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
	this->props = std::make_unique<std::vector<Prop>>();
	this->tileMap = std::unique_ptr<TileMap>{};
	this->actors = std::make_unique<std::vector<Actor>>();
	this->props = std::make_unique<std::vector<Prop>>();
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

	std::vector<int> startPosVec = roomJson.at("start_pos");
	startTile = Vector2{static_cast<float>(startPosVec[0]),
						static_cast<float>(startPosVec[1])};

	this->interactables = std::make_unique<InteractablesContainer>();
	this->collisions = std::make_unique<CollisionsContainer>();
	this->actors = std::make_unique<std::vector<Actor>>();
	this->props = std::make_unique<std::vector<Prop>>();

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

		collisions->addCollisionTile(x, y);
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

		std::vector<std::string> propVec = value;
		auto p = Prop(propVec.at(0));
		p.setWorldTilePos(Vector2{static_cast<float>(x), static_cast<float>(y)},
						  worldTileSize);

		propVec.erase(propVec.begin());
		inter_apply_vec(p.getInteractable(), propVec);

		addProp(std::move(p));
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
		addActor(std::move(a));
	}

	interactables->addJsonData(roomJson);

	musicSource = roomJson.at("music_source");

	UnloadFileText(jsonString);
}

Room::Room(std::unique_ptr<TileMap> tileMap) : Room() {
	this->tileMap = std::move(tileMap);
}

Room::Room(const RoomBin &bin) : Room() {
	this->worldTileSize = 48;

	this->interactables = std::make_unique<InteractablesContainer>();
	this->collisions = std::make_unique<CollisionsContainer>();
	this->actors = std::make_unique<std::vector<Actor>>();
	this->props = std::make_unique<std::vector<Prop>>();

	this->tileMap = std::make_unique<TileMap>(bin);

	auto actor = std::make_unique<Actor>("actors/playerActor.ractor");
	actor->setTilePosition(Vector2{static_cast<float>(bin.startPoint.x),
								   static_cast<float>(bin.startPoint.y)},
						   tileMap->getTileSet()->getTileSize());
	printf("%s \n", actor->getSourcePath().c_str());
	auto initialPlayer = std::make_unique<Player>(std::move(actor), *this);

	this->addPlayer(std::move(initialPlayer));

	interactables->addBinVector(bin.interactables);

	for (auto collisionBin : bin.collisions) {
		collisions->addCollisionTile(collisionBin.x, collisionBin.y);
	}

	for (auto const &propSource : bin.props) {
		for (auto const &propBin : Game::getBin().props) {
			if (propBin.name == propSource.name) {
				auto p = Prop(propBin);
				p.setWorldTilePos(
					Vector2{static_cast<float>(propSource.tilePos.x),
							static_cast<float>(propSource.tilePos.y)},
					worldTileSize);

				if (p.getHasInteractable()) {
					p.getInteractable()->setOnTouch(propSource.onTouch);

					InteractableBin intBin = {
						0, 0, static_cast<int>(p.getInteractable()->type),
						propSource.onTouch, propSource.dialogue};
					inter_apply_bin(p.getInteractable(), intBin);
				}
				addProp(std::move(p));
				break;
			}
		}
	}

	for (const auto &actorSource : bin.actors) {
		for (const auto &actorBin : Game::getBin().actors) {
			if (actorBin.name == actorSource.name) {
				auto a = Actor(actorBin);
				a.setTilePosition(
					Vector2{static_cast<float>(actorSource.tilePos.x),
							static_cast<float>(actorSource.tilePos.y)},
					Vector2{static_cast<float>(worldTileSize),
							static_cast<float>(worldTileSize)});
				addActor(std::move(a));
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
	for (Vector2 collisionPos : collisions->getVector()) {
		std::vector<int> collision;
		collision.push_back(static_cast<int>(collisionPos.x));
		collision.push_back(static_cast<int>(collisionPos.y));

		collisionsVector.push_back(collision);
	}
	roomJson.push_back({"collision", collisionsVector});

	// Vector for interactables
	auto interactablesProps = std::map<std::string, std::vector<std::string>>();

	auto interactablesVector = std::vector<std::vector<int>>();
	for (auto &&interactable : interactables->getList()) {
		std::vector<int> interactableVector;
		interactableVector.push_back(static_cast<int>(interactable->pos.x));
		interactableVector.push_back(static_cast<int>(interactable->pos.y));
		interactableVector.push_back(interactable->type);
		interactableVector.push_back(interactable->onTouch);

		std::vector<std::string> propsVec;
		if (interactable->type == INT_TWO) {
			auto *dialogueInter =
				dynamic_cast<IntBase<DiagInt> *>(interactable);
			std::string key =
				TextFormat("%i;%i", static_cast<int>(interactable->pos.x),
						   static_cast<int>(interactable->pos.y));
			propsVec.push_back(dialogueInter->get().dialogueSource);
			interactablesProps[key] = propsVec;
		}
		if (interactable->type == INT_WARPER) {
			auto *dialogueInter =
				dynamic_cast<IntBase<WarperInt> *>(interactable);
			std::string key =
				TextFormat("%i;%i", static_cast<int>(interactable->pos.x),
						   static_cast<int>(interactable->pos.y));
			propsVec.push_back(dialogueInter->get().targetRoom);
			interactablesProps[key] = propsVec;
		}

		interactablesVector.push_back(interactableVector);
	}

	auto propsMap = std::map<std::string, std::vector<std::string>>{};
	for (auto &&p : *props) {
		std::string key =
			TextFormat("%i;%i", static_cast<int>(p.getWorldTilePos().x),
					   static_cast<int>(p.getWorldTilePos().y));

		auto propVec = std::vector<std::string>{};
		propVec.push_back(p.getSourcePath());

		if (p.getHasInteractable()) {
			switch (p.getInteractable()->type) {
			case INT_TWO:
				propVec.push_back(
					(dynamic_cast<IntBase<DiagInt> *>(p.getInteractable()))
						->get()
						.dialogueSource);
				break;
			case INT_WARPER:
				propVec.push_back(
					(dynamic_cast<IntBase<WarperInt> *>(p.getInteractable()))
						->get()
						.targetRoom);
				break;
			default:
				break;
			}
		}

		propsMap[key] = propVec;
	}

	auto actorsMap = std::map<std::string, std::string>{};
	for (auto &&a : *actors) {
		std::string key =
			TextFormat("%i;%i", static_cast<int>(a.getTilePosition().x),
					   static_cast<int>(a.getTilePosition().y));

		actorsMap[key] = a.getSourcePath();
	}

	roomJson.push_back({"interactables", interactablesVector});
	roomJson.push_back({"interactable_props", interactablesProps});
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

	for (auto &&actor : *actors) {
		actor.unload();
	}

	player->unload();
}

void Room::update() {
	for (auto &&actor : *actors) {
		actor.update();
	}
	player->update();
	if (!lock)
		updateCamera();
}

void Room::updateCamera() {
	Vector2 playerPos = player->getPosition();
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
		auto rect = Rectangle{i->pos.x * static_cast<float>(getWorldTileSize()),
							  i->pos.y * static_cast<float>(getWorldTileSize()),
							  static_cast<float>(getWorldTileSize()),
							  static_cast<float>(getWorldTileSize())};
		DrawRectangleRec(rect, Fade(YELLOW, 0.5f));
	}
	for (auto c : collisions->getVector()) {
		auto rect = Rectangle{c.x * static_cast<float>(worldTileSize),
							  c.y * static_cast<float>(worldTileSize),
							  static_cast<float>(worldTileSize),
							  static_cast<float>(worldTileSize)};
		DrawRectangleRec(rect, Fade(RED, 0.5f));
	}

	for (auto &&p : *props) {
		if (p.getCollisionCenter().y <= player->getCollisionPos().y) {
			p.draw();
		}
	}

	for (auto &&actor : *actors) {
		actor.draw();
	}
	player->draw();

	for (auto &&p : *props) {
		if (p.getCollisionCenter().y > player->getCollisionPos().y) {
			p.draw();
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

std::vector<Vector2> Room::getCollisionTiles() const {
	return this->collisions->getVector();
}

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

std::vector<Prop> &Room::getProps() const { return *props; }

void Room::addProp(Prop prop) const { props->push_back(std::move(prop)); }

void Room::removeProp(Vector2 worldPos) const {
	for (auto it = props->begin(); it < props->end(); ++it) {
		if (it->getWorldTilePos().x == worldPos.x &&
			it->getWorldTilePos().y == worldPos.y) {
			props->erase(it);
		}
	}
}

Prop *Room::getPropAt(Vector2 worldPos) const {
	Prop *p = nullptr;
	int idx = 0;
	for (auto it = props->begin(); it < props->end(); ++it, idx++) {
		if (it->getWorldTilePos().x == worldPos.x &&
			it->getWorldTilePos().y == worldPos.y) {
			p = &props->at(idx);
		}
	}
	return p;
}

std::vector<Actor> &Room::getActors() const { return *actors; }

void Room::addActor(Actor actor) const {
	this->actors->push_back(std::move(actor));
}

void Room::removeActor(Vector2 tilePosition) const {
	for (auto it = actors->begin(); it < actors->end(); ++it) {
		if (it->getTilePosition().x == tilePosition.x &&
			it->getTilePosition().y == tilePosition.y) {
			actors->erase(it);
		}
	}
}
