#include "actions/placeTileAction.hpp"
#include "actions/mapAction.hpp"
#include "editor.hpp"
#include "prop.hpp"
#include "raylib.h"
#include "room.hpp"
#include "views/worldView.hpp"
#include <cstring>
#include <nlohmann/json_fwd.hpp>

PlaceTileAction::PlaceTileAction(MapActionData a) : MapAction(a) {}

void PlaceTileAction::execute() {
	switch (data.layer) {
	case RoomLayer::LAYER_TILES: {
		if (data.room->getTileMap()->getTileSet()->areAtlasCoordsValid(
				{static_cast<float>(data.tile.x),
				 static_cast<float>(data.tile.y)})) {
			data.room->getTileMap()->setTile(data.worldTile, data.tile);
		}
	} break;
	case RoomLayer::LAYER_COLLISION: {
		data.room->getCollisions().addCollisionTile(data.worldTile.x,
													data.worldTile.y);
	} break;
	case RoomLayer::LAYER_INTERACTABLES: {
		auto inter = data.room->getInteractables().add(
			data.worldTile.x, data.worldTile.y, data.interactable);
		if (inter != nullptr) {
			nlohmann::json interJson =
				json::parse(LoadFileText(data.interactableFullPath.c_str()));
			inter->setProps(interJson.at("props"));
		}
	} break;
	case RoomLayer::LAYER_PROPS: {
		Prop p(data.interactableFullPath);
		p.setWorldTilePos({data.worldTile.x, data.worldTile.y},
						  data.room->getWorldTileSize());
		if (p.getInteractable() != nullptr) {
			auto interType = p.getInteractable()->getType();
			auto interNames =
				Editor::instance->getProject()->getInteractableNames();
			std::string interFileName;
			bool found = false;
			for (auto [interPath, interName] : interNames) {
				std::string fileName = GetFileNameWithoutExt(interPath.c_str());
				if (fileName == interType) {
					found = true;
					interFileName = interPath;
				}
			}
			nlohmann::json propJson =
				json::parse(LoadFileText(interFileName.c_str()));
			p.getInteractable()->setProps(propJson.at("props"));
		}
		data.room->addProp(std::move(p));
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
		data.room->getCollisions().removeCollisionTile(data.worldTile.x,
													   data.worldTile.y);
	} break;
	case RoomLayer::LAYER_INTERACTABLES: {
		data.room->getInteractables().removeInteractable(data.worldTile.x,
														 data.worldTile.y);
	} break;
	case RoomLayer::LAYER_PROPS: {
		data.room->removeProp({static_cast<float>(data.worldTile.x),
							   static_cast<float>(data.worldTile.y)});
	} break;
	default:
		break;
	}
}
