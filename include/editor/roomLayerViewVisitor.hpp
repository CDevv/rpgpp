#ifndef _RPGPP_ROOMLAYERVIEWVISITOR_H
#define _RPGPP_ROOMLAYERVIEWVISITOR_H

#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "interactable.hpp"
#include "views/tileSetView.hpp"
#include "views/worldView.hpp"
#include <enum_visitor/enum_visitor.hpp>

class RoomLayerViewVisitor
	: public mj::enum_visitor<
		  RoomLayer, RoomLayer::LAYER_TILES, RoomLayer::LAYER_COLLISION,
		  RoomLayer::LAYER_INTERACTABLES, RoomLayer::LAYER_PROPS> {
  public:
	RoomLayerViewVisitor();
	tgui::Group::Ptr group{nullptr};
	RoomTool tool;
	Interactable *inter{nullptr};
	void operator()(enum_v<RoomLayer::LAYER_COLLISION>);
	void operator()(enum_v<RoomLayer::LAYER_TILES>);
	void operator()(enum_v<RoomLayer::LAYER_INTERACTABLES>);
	void operator()(enum_v<RoomLayer::LAYER_PROPS>);

	TileSetView::Ptr tileSetView;
	tgui::ComboBox::Ptr interactableChoose;
	tgui::ComboBox::Ptr propChoose;
};

#endif
