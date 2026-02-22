#include "views/roomView.hpp"
#include "TGUI/Event.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "actions/action.hpp"
#include "actions/editTileAction.hpp"
#include "actions/eraseTileAction.hpp"
#include "actions/mapAction.hpp"
#include "actions/placeTileAction.hpp"
#include "actor.hpp"
#include "editor.hpp"
#include "enum_visitor/enum_visitor.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "raylib.h"
#include "room.hpp"
#include "screens/projectScreen.hpp"
#include "services/fileSystemService.hpp"
#include "tile.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include "views/worldView.hpp"
#include <cmath>
#include <memory>
#include <utility>

RoomView::RoomView() { layer = RoomLayer::LAYER_TILES; }

RoomView::Ptr RoomView::create() { return std::make_shared<RoomView>(); }

RoomView::Ptr RoomView::create(Room *room) {
	auto ptr = std::make_shared<RoomView>();
	ptr->room = room;
	return ptr;
}

void RoomView::setRoom(Room *room) { this->room = room; }

Room *RoomView::getRoom() { return room; }

IVector RoomView::getTileAtMouse() {
	TileMap *tileMap = room->getTileMap();
	return {static_cast<int>(
				std::floor(mouseWorldPos.x / tileMap->getWorldTileSize())),
			static_cast<int>(
				std::floor(mouseWorldPos.y / tileMap->getWorldTileSize()))};
}

Rectangle RoomView::getSourceRect(TileMap *tileMap, int x, int y) {
	Rectangle sourceRect = {static_cast<float>(x * tileMap->getAtlasTileSize()),
							static_cast<float>(y * tileMap->getAtlasTileSize()),
							static_cast<float>(tileMap->getAtlasTileSize()),
							static_cast<float>(tileMap->getAtlasTileSize())};
	return sourceRect;
}

Rectangle RoomView::getDestRect(TileMap *tileMap, int x, int y) {
	Rectangle destRect = {static_cast<float>(x * tileMap->getWorldTileSize()),
						  static_cast<float>(y * tileMap->getWorldTileSize()),
						  static_cast<float>(tileMap->getWorldTileSize()),
						  static_cast<float>(tileMap->getWorldTileSize())};
	return destRect;
}

void RoomView::setBrush(bool value) { this->brushMode = value; }

void RoomView::setLayer(RoomLayer layer) { this->layer = layer; }

void RoomView::drawOverlay() {
	if (room == nullptr) {
		return;
	}

	TileMap *tileMap = room->getTileMap();
	Vector2 worldSize = tileMap->getMaxWorldSize();
	Vector2 actualSize = worldSize;
	actualSize.x *= tileMap->getWorldTileSize();
	actualSize.y *= tileMap->getWorldTileSize();

	Rectangle rect = {0, 0, actualSize.x, actualSize.y};

	if (CheckCollisionPointRec(mouseWorldPos, rect)) {
		int tileX = std::floor(mouseWorldPos.x / tileMap->getWorldTileSize());
		int tileY = std::floor(mouseWorldPos.y / tileMap->getWorldTileSize());
		DrawText(TextFormat("Tile [%i, %i]", tileX, tileY), 8, 8, 36, BLACK);
	}
}

void RoomView::drawCanvas() {
	ClearBackground(RAYWHITE);

	if (room == nullptr) {
		return;
	}

	TileMap *tileMap = room->getTileMap();
	if (tileMap == nullptr) {
		return;
	}
	TileSet *tileSet = tileMap->getTileSet();
	if (tileSet == nullptr) {
		return;
	}

	const Texture2D &texture = tileSet->getTexture();

	Vector2 worldSize = tileMap->getMaxWorldSize();
	Vector2 tileSize = {static_cast<float>(tileMap->getWorldTileSize()),
						static_cast<float>(tileMap->getWorldTileSize())};
	Vector2 atlasTileSize = {static_cast<float>(tileMap->getAtlasTileSize()),
							 static_cast<float>(tileMap->getAtlasTileSize())};
	int worldWidth = static_cast<int>(worldSize.x);
	int worldHeight = static_cast<int>(worldSize.y);
	for (int tileX = 0; tileX < worldWidth; tileX++) {
		for (int tileY = 0; tileY < worldHeight; tileY++) {
			auto tile = tileMap->getTile(tileX, tileY);
			auto atlasTile = tile.getAtlasTile();
			auto atlas = atlasTile.getAtlasCoords();

			Rectangle sourceRect = getSourceRect(tileMap, atlas.x, atlas.y);
			Rectangle destRect = getDestRect(tileMap, tileX, tileY);

			if (tile.isPlaced()) {
				// Draw tile itself
				DrawTexturePro(texture, sourceRect, destRect, {0.0f, 0.0f},
							   0.0f, WHITE);
			}

			if (tileSetView != nullptr) {
				handleMode(tileX, tileY);
			}

			// Draw tile border
			DrawRectangleLinesEx(destRect, 1.0f, Fade(GRAY, 0.5f));
			if (CheckCollisionPointRec(mouseWorldPos, destRect)) {
				DrawRectangleLinesEx(destRect, 2.0f, Fade(GRAY, 0.5f));
			}
		}
	}

	// collisions
	for (auto collision : room->getCollisions().getVector()) {
		int tileX = static_cast<int>(collision.x);
		int tileY = static_cast<int>(collision.y);

		Rectangle destRect = getDestRect(tileMap, tileX, tileY);

		DrawRectangleRec(destRect, Fade(RED, 0.2f));
	}

	// interactables
	auto interactableNames =
		Editor::instance->getProject()->getInteractableNames();
	for (auto interactable : room->getInteractables().getList()) {
		int tileX = static_cast<int>(interactable->getWorldPos().x);
		int tileY = static_cast<int>(interactable->getWorldPos().y);

		Rectangle destRect = getDestRect(tileMap, tileX, tileY);

		std::string shownTitle = interactable->getType();

		if (!interactableNames.empty()) {
			if (interactableNames.count(interactable->getType()) > 0) {
				shownTitle = interactableNames[interactable->getType()];
			}
		}

		DrawRectangleRec(destRect, Fade(YELLOW, 0.2f));
		DrawText(TextFormat("%s", shownTitle.c_str()),
				 static_cast<int>(destRect.x), static_cast<int>(destRect.y), 16,
				 ORANGE);

		if (CheckCollisionPointRec(mouseWorldPos, destRect)) {
			// auto tooltip = tgui::Label::create("Interactable");
		}
	}

	// props
	for (auto &&prop : room->getProps()) {
		auto pos = prop.getWorldPos();
		auto texture = prop.getTexture();

		Rectangle source = {0, 0, static_cast<float>(texture.width),
							static_cast<float>(texture.height)};
		Rectangle dest = {
			pos.x, pos.y,
			static_cast<float>(texture.width) * RPGPP_DRAW_MULTIPLIER,
			static_cast<float>(texture.height) * RPGPP_DRAW_MULTIPLIER};

		DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, 0.0f,
					   Fade(WHITE, 0.5f));
	}

	DrawCircleV(getMouseWorldPos(), 1.0f, MAROON);
}

void RoomView::mouseMoved(tgui::Vector2f pos) {
	if (brushMode && mouseLeftButton) {
		handleModePress(pos);
	}
	WorldView::mouseMoved(pos);
}

bool RoomView::leftMousePressed(tgui::Vector2f pos) {
	TileMap *tileMap = room->getTileMap();

	handleModePress(pos);

	return WorldView::leftMousePressed(pos);
}

void RoomView::keyPressed(const tgui::Event::KeyEvent &event) {
	if (event.control) {
		auto screen = aurora::downcast<screens::ProjectScreen *>(
			Editor::instance->getGui().currentScreen.get());
		if (event.code == tgui::Event::KeyboardKey::Z) {
			screen->getCurrentFile().getView().undoAction();
		}
		if (event.code == tgui::Event::KeyboardKey::C) {
			screen->getCurrentFile().getView().redoAction();
		}
	}
}

void RoomView::handleMode(int x, int y) {
	switch (tool) {
	case RoomTool::TOOL_PLACE:
		handlePlaceMode(x, y);
		break;
	case RoomTool::TOOL_ERASE:
		handleEraseMode(x, y);
		break;
	case RoomTool::TOOL_EDIT:
		handleEditMode(x, y);
		break;
	default:
		break;
	}
}

void RoomView::handlePlaceMode(int x, int y) {
	TileMap *tileMap = room->getTileMap();
	TileSet *tileSet = tileMap->getTileSet();
	const Texture2D &texture = tileSet->getTexture();

	Rectangle destRect = getDestRect(tileMap, x, y);

	if (CheckCollisionPointRec(mouseWorldPos, destRect)) {
		auto atlasTilePos = tileSetView->getSelectedTile();

		switch (layer) {
		case RoomLayer::LAYER_TILES: {
			if (tileSet->areAtlasCoordsValid(
					{static_cast<float>(atlasTilePos.x),
					 static_cast<float>(atlasTilePos.y)})) {
				Rectangle atlasSourceRect =
					getSourceRect(tileMap, atlasTilePos.x, atlasTilePos.y);

				IVector tileMouse = getTileAtMouse();
				Rectangle destRect =
					getDestRect(tileMap, tileMouse.x, tileMouse.y);

				DrawTexturePro(texture, atlasSourceRect, destRect, {0.0f, 0.0f},
							   0.0f, Fade(WHITE, 0.7f));
			}
		} break;
		case RoomLayer::LAYER_COLLISION: {
			IVector tileMouse = getTileAtMouse();
			Rectangle destRect = getDestRect(tileMap, tileMouse.x, tileMouse.y);
			DrawRectangleRec(destRect, Fade(RED, 0.7f));
		} break;
		case RoomLayer::LAYER_INTERACTABLES: {
			IVector tileMouse = getTileAtMouse();
			Rectangle destRect = getDestRect(tileMap, tileMouse.x, tileMouse.y);
			DrawRectangleRec(destRect, Fade(YELLOW, 0.7f));
		}
		default:
			break;
		}
	}
}

void RoomView::handleEditMode(int x, int y) {
	TileMap *tileMap = room->getTileMap();
	Rectangle destRect = getDestRect(tileMap, x, y);

	if (selectedTile.x == x && selectedTile.y == y) {
		Rectangle selectedTileRect = getDestRect(tileMap, x, y);
		DrawRectangleRec(selectedTileRect, Fade(BLUE, 0.3f));
	}

	if (CheckCollisionPointRec(mouseWorldPos, destRect)) {
		DrawRectangleLinesEx(destRect, 2.0f, ORANGE);
	}
}

void RoomView::handleEraseMode(int x, int y) {
	TileMap *tileMap = room->getTileMap();
	Rectangle destRect = getDestRect(tileMap, x, y);

	if (CheckCollisionPointRec(mouseWorldPos, destRect)) {
		DrawRectangleRec(destRect, Fade(RED, 0.3f));
	}
}

void RoomView::handleModePress(tgui::Vector2f pos) {
	auto screen = aurora::downcast<screens::ProjectScreen *>(
		Editor::instance->getGui().currentScreen.get());

	IVector tileMouse = getTileAtMouse();
	IVector atlasTilePos = tileSetView->getSelectedTile();

	MapActionData data;
	data.view = this;
	data.room = room;
	data.layer = layer;
	data.tile = {static_cast<float>(atlasTilePos.x),
				 static_cast<float>(atlasTilePos.y)};
	data.worldTile = {static_cast<float>(tileMouse.x),
					  static_cast<float>(tileMouse.y)};
	if (layer == RoomLayer::LAYER_INTERACTABLES) {
		data.interactable = GetFileNameWithoutExt(
			interactableChoose->getSelectedItemId().toStdString().c_str());
		data.interactableFullPath =
			interactableChoose->getSelectedItemId().toStdString();
	} else {
		data.interactable = GetFileNameWithoutExt(
			propChoose->getSelectedItemId().toStdString().c_str());
		data.interactableFullPath =
			propChoose->getSelectedItemId().toStdString();
	}

	switch (tool) {
	case RoomTool::TOOL_PLACE: {
		std::unique_ptr<Action> act = std::make_unique<PlaceTileAction>(data);

		screen->getCurrentFile().getView().pushAction(std::move(act));
	} break;
	case RoomTool::TOOL_ERASE: {
		std::unique_ptr<Action> act = std::make_unique<EraseTileAction>(data);

		screen->getCurrentFile().getView().pushAction(std::move(act));
	} break;
	case RoomTool::TOOL_EDIT:
		handleEditPress(pos);
		break;
	default:
		break;
	}
}

void RoomView::handleEditPress(tgui::Vector2f pos) {
	auto screen = aurora::downcast<screens::ProjectScreen *>(
		Editor::instance->getGui().currentScreen.get());

	TileMap *tileMap = room->getTileMap();

	selectedTile = getTileAtMouse();
	if (!tileMap->worldPosIsValid({static_cast<float>(selectedTile.x),
								   static_cast<float>(selectedTile.y)})) {
		return;
	}
	Vector2 atlasCoords = tileMap->getTile(selectedTile.x, selectedTile.y)
							  .getAtlasTile()
							  .getAtlasCoords();
	IVector atlasCoordsInt = {static_cast<int>(atlasCoords.x),
							  static_cast<int>(atlasCoords.y)};

	switch (layer) {
	case RoomLayer::LAYER_TILES: {
		tileSetView->setSelectedTile(atlasCoordsInt);
		tileSetView->onTileSelected.disconnectAll();
		tileSetView->onTileSelected([this, tileMap, screen](IVector newTile) {
			IVector tileMouse = selectedTile;

			MapActionData data;
			data.room = room;
			data.layer = layer;
			data.tile = {static_cast<float>(newTile.x),
						 static_cast<float>(newTile.y)};
			data.worldTile = {static_cast<float>(tileMouse.x),
							  static_cast<float>(tileMouse.y)};
			data.interactable = static_cast<InteractableType>(
				interactableChoose->getSelectedItemIndex() + 1);

			std::unique_ptr<Action> act =
				std::make_unique<EditTileAction>(data);
			screen->getCurrentFile().getView().pushAction(std::move(act));
		});
	} break;
	case RoomLayer::LAYER_INTERACTABLES: {
		IVector tileMouse = getTileAtMouse();
		layerVisitor->inter =
			room->getInteractables().getInt(tileMouse.x, tileMouse.y);
		layerVisitor->group->removeAllWidgets();
		mj::visit(*layerVisitor, layer);
	} break;
	default:
		break;
	}
}
