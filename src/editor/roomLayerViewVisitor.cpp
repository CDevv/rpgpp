#include "roomLayerViewVisitor.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "roomToolbox.hpp"

RoomLayerViewVisitor::RoomLayerViewVisitor() {
	tileSetView = TileSetView::create();
	tileSetView->setSize({"100%", "100%"});

	interactableChoose = tgui::ComboBox::create();
	interactableChoose->setDefaultText("Dialogue");
	interactableChoose->addItem("Dialogue");
	interactableChoose->addItem("Warper");
	interactableChoose->setSelectedItemByIndex(0);
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_TILES>) {
	group->add(tileSetView);
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_COLLISION>) {
	group->add(tgui::Label::create("Collision Layer"));
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_INTERACTABLES>) {
	group->add(tgui::Label::create("Interactables"));

	group->add(interactableChoose);
}
