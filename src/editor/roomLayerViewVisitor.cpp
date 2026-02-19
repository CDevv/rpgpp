#include "roomLayerViewVisitor.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include <raylib.h>

RoomLayerViewVisitor::RoomLayerViewVisitor() {
	tileSetView = TileSetView::create();
	tileSetView->setSize({"100%", "100%"});

	interactableChoose = tgui::ComboBox::create();
	interactableChoose->setDefaultText("Dialogue");

	auto map = Editor::instance->getProject()->getInteractableNames();
	interactableChoose->setSelectedItemByIndex(0);
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_TILES>) {
	group->add(tileSetView);
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_COLLISION>) {
	group->add(tgui::Label::create("Collision Layer"));
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_INTERACTABLES>) {
	interactableChoose->removeAllItems();
	auto map = Editor::instance->getProject()->getInteractableNames();
	for (auto &[key, val] : map) {
		interactableChoose->addItem(val, GetFileNameWithoutExt(key.c_str()));
	}
	interactableChoose->setSelectedItemByIndex(0);

	group->add(tgui::Label::create("Interactables"));

	group->add(interactableChoose);
}
