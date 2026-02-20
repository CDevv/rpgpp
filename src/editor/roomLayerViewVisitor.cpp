#include "roomLayerViewVisitor.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "views/worldView.hpp"
#include <cstdio>
#include <raylib.h>
#include <string>

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
	if (tool == RoomTool::TOOL_PLACE) {
		interactableChoose->removeAllItems();
		auto map = Editor::instance->getProject()->getInteractableNames();
		for (auto &[key, val] : map) {
			interactableChoose->addItem(val, key.c_str());
		}
		interactableChoose->setSelectedItemByIndex(0);

		group->add(tgui::Label::create("Interactables"));

		group->add(interactableChoose);
	} else if (tool == RoomTool::TOOL_EDIT) {
		if (inter == nullptr) {
			group->add(tgui::Label::create("Interactables"));
		} else {
			if (inter->getProps().is_object()) {
				for (auto item : inter->getProps().items()) {
					printf("%s \n", item.key().c_str());
					if (item.value().is_string()) {
						printf("%s \n",
							   item.value().get<std::string>().c_str());
					}
				}
			}
		}
	} else {
		group->add(tgui::Label::create("Interactables"));
	}
}
