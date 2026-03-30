#ifndef _RPGPP_ROOMLAYERVIEWVISITOR_H
#define _RPGPP_ROOMLAYERVIEWVISITOR_H

#include <raylib.h>

#include <enum_visitor/enum_visitor.hpp>
#include <memory>

#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "actor.hpp"
#include "interactable.hpp"
#include "prop.hpp"
#include "views/tileSetView.hpp"
#include "views/worldView.hpp"

class RoomLayerViewVisitor
	: public mj::enum_visitor<RoomLayer, RoomLayer::LAYER_TILES, RoomLayer::LAYER_COLLISION,
							  RoomLayer::LAYER_INTERACTABLES, RoomLayer::LAYER_PROPS, RoomLayer::LAYER_ACTORS> {
public:
	RoomLayerViewVisitor();
	tgui::Group::Ptr group{nullptr};
	RoomTool tool;
	Interactable *inter{nullptr};
	Prop *prop{nullptr};
	bool isAvailable = true;
	Texture2D propTexture{};
	Texture2D actorTexture{};
	std::unique_ptr<Actor> chosenActor;
	void operator()(enum_v<RoomLayer::LAYER_COLLISION>);
	void operator()(enum_v<RoomLayer::LAYER_TILES>);
	void operator()(enum_v<RoomLayer::LAYER_INTERACTABLES>);
	void operator()(enum_v<RoomLayer::LAYER_PROPS>);
	void operator()(enum_v<RoomLayer::LAYER_ACTORS>);

	TileSetView::Ptr tileSetView;
	tgui::ComboBox::Ptr interactableChoose;
	tgui::ComboBox::Ptr propChoose;
	tgui::ComboBox::Ptr actorChoose;
	tgui::EditBox::Ptr actorNameInput;

	~RoomLayerViewVisitor();
};

#endif
