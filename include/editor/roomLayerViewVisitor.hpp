#ifndef _RPGPP_ROOMLAYERVIEWVISITOR_H
#define _RPGPP_ROOMLAYERVIEWVISITOR_H

#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "roomToolbox.hpp"
#include "tileSetView.hpp"
#include <enum_visitor/enum_visitor.hpp>

class RoomLayerViewVisitor
	: public mj::enum_visitor<RoomLayer, RoomLayer::LAYER_TILES,
							  RoomLayer::LAYER_COLLISION,
							  RoomLayer::LAYER_INTERACTABLES> {
  public:
	RoomLayerViewVisitor();
	tgui::Group::Ptr group{nullptr};
	void operator()(enum_v<RoomLayer::LAYER_COLLISION>);
	void operator()(enum_v<RoomLayer::LAYER_TILES>);
	void operator()(enum_v<RoomLayer::LAYER_INTERACTABLES>);

	TileSetView::Ptr tileSetView;
	tgui::ComboBox::Ptr interactableChoose;
};

#endif