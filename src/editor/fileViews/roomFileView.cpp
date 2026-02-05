#include "fileViews/roomFileView.hpp"
#include "TGUI/String.hpp"
#include "editor.hpp"
#include "propertiesBox.hpp"
#include "room.hpp"
#include "tileSetView.hpp"
#include "widgets/fileField.hpp"
#include "widgets/roomToolbox.hpp"
#include "widgets/roomView.hpp"

RoomFileView::RoomFileView() {
	roomView = RoomView::create();
	roomView->setSize({"100% - 300", "100% - 32"});
	roomView->setPosition(0, 32);
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(roomView));
	widgetContainer.push_back(roomView);

	tileSetView = TileSetView::create();
	tileSetView->setPosition("100% - 300", 0);
	tileSetView->setSize({"300", "300"});
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(tileSetView));
	widgetContainer.push_back(tileSetView);

	roomView->tileSetView = tileSetView.get();

	auto props = PropertiesBox::create();
	props->setSize({"300", "100%"});
	props->setPosition({"100% - 300", 300});

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
	toolbox->setSize({164, 32});
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