#include "actions/placeTileAction.hpp"

#include <memory>
#include <nlohmann/json_fwd.hpp>

#include "actions/mapAction.hpp"
#include "actor.hpp"
#include "conversion.hpp"
#include "editor.hpp"
#include "prop.hpp"
#include "raylib.h"
#include "raymath.h"
#include "room.hpp"
#include "views/worldView.hpp"

PlaceTileAction::PlaceTileAction(MapActionData a) : MapAction(a) {}

void PlaceTileAction::execute() {
	switch (data.layer) {
		case RoomLayer::LAYER_TILES: {
			if (data.room->getTileMap()->getTileSet()->areAtlasCoordsValid(
					{static_cast<float>(data.tile.x), static_cast<float>(data.tile.y)})) {
				data.room->getTileMap()->setTile(data.worldTile, data.tile);
			}
		} break;
		case RoomLayer::LAYER_COLLISION: {
			auto conv = fromVector2(data.worldTile);
			data.room->getCollisions().pushObject(fromVector2(data.worldTile), false);
		} break;
		case RoomLayer::LAYER_INTERACTABLES: {
			auto inter = data.room->getInteractables().add(fromVector2(data.worldTile), data.interactable);
			if (inter != nullptr) {
				char *txt = LoadFileText(data.interactableFullPath.c_str());
				nlohmann::json interJson = json::parse(txt);
				UnloadFileText(txt);
				inter->setProps(interJson.at("props"));
				inter->setOnTouch(interJson.at("onTouch"));
			}
		} break;
		case RoomLayer::LAYER_PROPS: {
			auto p = std::make_unique<Prop>(data.interactableFullPath);
			p->setWorldTilePos({data.worldTile.x, data.worldTile.y}, data.room->getWorldTileSize());
			if (p->getInteractable() != nullptr && p->getHasInteractable()) {
				auto interType = p->getInteractable()->getType();
				auto interNames = Editor::instance->getProject()->getInteractableNames();
				std::string interFileName;
				bool found = false;
				for (auto [interPath, interName] : interNames) {
					std::string fileName = GetFileNameWithoutExt(interPath.c_str());
					if (fileName == interType) {
						found = true;
						interFileName = interPath;
					}
				}

				char *txt = LoadFileText(interFileName.c_str());
				nlohmann::json propJson = json::parse(txt);
				UnloadFileText(txt);
				p->getInteractable()->setProps(propJson.at("props"));
			}
			// data.room->addProp(std::move(p));
			data.room->getProps().pushObject(fromVector2(data.worldTile), std::move(p));
		} break;
		case RoomLayer::LAYER_ACTORS: {
			auto a = std::make_unique<Actor>(data.interactableFullPath);
			a->setTilePosition(data.worldTile, data.room->getTileMap()->getTileSet()->getTileSize());
			data.room->getActors().getActors()[data.actorName] = std::move(a);
		} break;
		default:
			break;
	}
}

void PlaceTileAction::undo() {
	switch (data.layer) {
		case RoomLayer::LAYER_TILES: {
			data.room->getTileMap()->setTile(data.worldTile, data.prevTile);
		} break;
		case RoomLayer::LAYER_COLLISION: {
			data.room->getCollisions().removeObject(fromVector2(data.worldTile));
		} break;
		case RoomLayer::LAYER_INTERACTABLES: {
			data.room->getInteractables().removeObject(fromVector2(data.worldTile));
		} break;
		case RoomLayer::LAYER_PROPS: {
			data.room->getProps().removeObject(fromVector2(data.worldTile));
		} break;
		case RoomLayer::LAYER_ACTORS: {
			for (auto &&a : data.room->getActors().getActors()) {
				if (Vector2Equals(a.second->getTilePosition(), data.worldTile)) {
					data.room->getActors().getActors().erase(a.first);
					break;
				}
			}
		} break;
		default:
			break;
	}
}
