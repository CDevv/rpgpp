#include "fileViews/roomFileView.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widgets/CheckBox.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Scrollbar.hpp"
#include "editor.hpp"
#include "enum_visitor/enum_visitor.hpp"
#include "fileViews/fileView.hpp"
#include "raylib.h"
#include "room.hpp"
#include "roomViewModesHandler.hpp"
#include "views/roomView.hpp"
#include "views/tileSetView.hpp"
#include "views/worldView.hpp"
#include "widgets/fileField.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/toolbox.hpp"
#include <memory>

RoomFileView::RoomFileView() {
	RoomTool a;
	TranslationService &ts = Editor::instance->getTranslations();

	roomView = RoomView::create();
	roomView->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W),
					   TextFormat("100%% - %d", TOOLBOX_H)});
	roomView->setPosition(0, TOOLBOX_H);
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(roomView));
	widgetContainer.push_back(roomView);

	auto roomLayerGroup = tgui::Group::create();
	roomLayerGroup->setPosition(TextFormat("100%% - %d", RIGHT_PANEL_W),
								LAYER_CHOOSE_H);
	roomLayerGroup->setSize({ROOM_LAYER_W, ROOM_LAYER_H});

	layerVisitor.group = roomLayerGroup;
	roomView->layerVisitor = &layerVisitor;

	tileSetView = layerVisitor.tileSetView;
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(tileSetView));
	roomLayerGroup->add(tileSetView);
	widgetContainer.push_back(roomLayerGroup);

	roomView->tileSetView = tileSetView.get();
	roomView->interactableChoose = layerVisitor.interactableChoose.get();
	roomView->propChoose = layerVisitor.propChoose.get();

	modesHandler = std::make_unique<RoomViewModesHandler>();
	modesHandler->view = roomView;
	roomView->fileView = dynamic_cast<FileView *>(this);

	layerChoose = tgui::ComboBox::create();
	layerChoose->setPosition(TextFormat("100%% - %d", RIGHT_PANEL_W), 0);
	layerChoose->setSize(RIGHT_PANEL_W, LAYER_CHOOSE_H);
	layerChoose->addItem("Tiles");
	layerChoose->addItem("Collisions");
	layerChoose->addItem("Interactables");
	layerChoose->addItem("Props");
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
	props->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W),
						ROOM_LAYER_H + LAYER_CHOOSE_H});

	widthField = IntField::create();
	widthField->label->setText(ts.getKey("screen.project.roomview.mapwidth"));
	widthField->value->onValueChange([this](int value) {
		Vector2 worldSize =
			this->roomView->getRoom()->getTileMap()->getMaxWorldSize();
		worldSize.x = value;
		this->roomView->getRoom()->getTileMap()->setWorldSize(worldSize);
	});
	props->addIntField(widthField);

	heightField = IntField::create();
	heightField->label->setText(ts.getKey("screen.project.roomview.mapheight"));
	heightField->value->onValueChange([this](int value) {
		Vector2 worldSize =
			this->roomView->getRoom()->getTileMap()->getMaxWorldSize();
		worldSize.y = value;
		this->roomView->getRoom()->getTileMap()->setWorldSize(worldSize);
	});
	props->addIntField(heightField);

	tileSetField = FileField::create(
		ts.getKey("screen.project.roomview.tileset_file"), "...");
	tileSetField->pathFilters = {{"RPG++ TileSet", {"*.rtiles"}}};
	tileSetField->callback = [this](const tgui::String &path) {
		auto room = this->roomView->getRoom();
		room->getTileMap()->setTileSet(path.toStdString());
	};
	props->addFileField(tileSetField);

	widgetContainer.push_back(props);

	auto toolbox = Toolbox<RoomTool>::create();
	toolbox->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
	toolbox->getHorizontalScrollbar()->setPolicy(
		tgui::Scrollbar::Policy::Never);
	toolbox->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), TOOLBOX_H});
	toolbox->addTool(ToolboxItem<RoomTool>{"tool", RoomTool::TOOL_NONE, "Mouse",
										   "tool_none.png"});
	toolbox->addTool(ToolboxItem<RoomTool>{"tool", RoomTool::TOOL_PLACE,
										   "Place", "tool_place.png"});
	toolbox->addTool(ToolboxItem<RoomTool>{"tool", RoomTool::TOOL_ERASE,
										   "Erase", "tool_erase.png"});
	toolbox->addTool(ToolboxItem<RoomTool>{"tool", RoomTool::TOOL_EDIT, "Edit",
										   "tool_edit.png"});

	auto brushToggle = tgui::CheckBox::create("Enable brush mode");
	brushToggle->onChange([this, brushToggle]() {
		roomView->setBrush(brushToggle->isChecked());
	});
	auto brushToggleSize =
		TOOLBOX_H - toolbox->getRenderer()->getPadding().getTop();
	brushToggle->setSize({brushToggleSize, brushToggleSize});
	toolbox->addWidget(brushToggle);

	toolbox->onItemClicked([this](ToolboxItem<RoomTool> tool) {
		tileSetView->setTool(tool.id);
		roomView->setTool(tool.id);
		layerVisitor.tool = tool.id;
		layerVisitor.group->removeAllWidgets();
		mj::visit(layerVisitor,
				  static_cast<RoomLayer>(layerChoose->getSelectedItemIndex()));
		cout << "Selected tool: " << tool.text << endl;
	});
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
