#include "fileViews/roomFileView.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "editor.hpp"
#include "enum_visitor/enum_visitor.hpp"
#include "fileViews/fileView.hpp"
#include "propertiesBox.hpp"
#include "raylib.h"
#include "room.hpp"
#include "roomViewModesHandler.hpp"
#include "tileSetView.hpp"
#include "widgets/fileField.hpp"
#include "widgets/roomToolbox.hpp"
#include "widgets/roomView.hpp"
#include <memory>

RoomFileView::RoomFileView() {
	roomView = RoomView::create();
	roomView->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), TextFormat("100%% - %d", TOOLBOX_H)});
	roomView->setPosition(0, TOOLBOX_H);
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(roomView));
	widgetContainer.push_back(roomView);

	auto roomLayerGroup = tgui::Group::create();
	roomLayerGroup->setPosition(TextFormat("100%% - %d", RIGHT_PANEL_W), LAYER_CHOOSE_H);
	roomLayerGroup->setSize({ROOM_LAYER_W, ROOM_LAYER_H});

	layerVisitor.group = roomLayerGroup;

	tileSetView = layerVisitor.tileSetView;
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(tileSetView));
	roomLayerGroup->add(tileSetView);
	widgetContainer.push_back(roomLayerGroup);

	roomView->tileSetView = tileSetView.get();
	roomView->interactableChoose = layerVisitor.interactableChoose.get();

	modesHandler = std::make_unique<RoomViewModesHandler>();
	modesHandler->view = roomView;
	roomView->fileView = dynamic_cast<FileView *>(this);

	auto layerChoose = tgui::ComboBox::create();
	layerChoose->setPosition(TextFormat("100%% - %d", RIGHT_PANEL_W), 0);
	layerChoose->setSize(RIGHT_PANEL_W, LAYER_CHOOSE_H);
	layerChoose->addItem("Tiles");
	layerChoose->addItem("Collisions");
	layerChoose->addItem("Interactables");
	layerChoose->setSelectedItemByIndex(0);
	widgetContainer.push_back(layerChoose);

	layerChoose->onItemSelect([this, roomLayerGroup](int index) {
		auto layerEnum = static_cast<RoomLayer>(index);
		roomView->setLayer(layerEnum);

		roomLayerGroup->removeAllWidgets();
		mj::visit(layerVisitor, layerEnum);
	});

	auto props = PropertiesBox::create();
	props->setSize({RIGHT_PANEL_W, "100%"});
	props->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), ROOM_LAYER_H + LAYER_CHOOSE_H});

	widthField = IntField::create();
	widthField->label->setText("Map Height");
	widthField->value->onValueChange([this](int value) {
		Vector2 worldSize =
			this->roomView->getRoom()->getTileMap()->getMaxWorldSize();
		worldSize.x = value;
		this->roomView->getRoom()->getTileMap()->setWorldSize(worldSize);
	});
	props->addIntField(widthField);

	heightField = IntField::create();
	heightField->label->setText("Map Height");
	heightField->value->onValueChange([this](int value) {
		Vector2 worldSize =
			this->roomView->getRoom()->getTileMap()->getMaxWorldSize();
		worldSize.y = value;
		this->roomView->getRoom()->getTileMap()->setWorldSize(worldSize);
	});
	props->addIntField(heightField);

	tileSetField = FileField::create("TileSet", "...");
	tileSetField->pathFilters = {{"RPG++ TileSet", {"*.rtiles"}}};
	tileSetField->callback = [this](const tgui::String &path) {
		auto room = this->roomView->getRoom();
		room->getTileMap()->setTileSet(path.toStdString());
	};
	props->addFileField(tileSetField);

	widgetContainer.push_back(props);

	auto toolbox = RoomToolbox::create();
	toolbox->setSize({164, TOOLBOX_H});
	toolbox->addTool("tool_none.png", RoomTool::TOOL_NONE);
	toolbox->addTool("tool_place.png", RoomTool::TOOL_PLACE);
	toolbox->addTool("tool_erase.png", RoomTool::TOOL_ERASE);
	toolbox->addTool("tool_edit.png", RoomTool::TOOL_EDIT);
	toolbox->onToolPressed([this](RoomTool tool) {
		tileSetView->setTool(tool);
		roomView->setTool(tool);
	});
	toolbox->onBrushPressed(
		[this](bool brushMode) { roomView->setBrush(brushMode); });
	widgetContainer.push_back(toolbox);
}

void RoomFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	if (variant == nullptr)
		return;

	auto ptr = dynamic_cast<Variant<Room> *>(variant);
	auto room = ptr->get();
	if (room == nullptr)
		return;

	roomView->setRoom(room);
	tileSetView->setTileSet(room->getTileMap()->getTileSet());
	widthField->value->setValue(room->getTileMap()->getMaxWorldSize().x);
	heightField->value->setValue(room->getTileMap()->getMaxWorldSize().y);
	tileSetField->value->setText(room->getTileMap()->getTileSetSource());

	addWidgets(layout);
}
