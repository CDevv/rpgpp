#include "roomLayerViewVisitor.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "views/worldView.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/textField.hpp"
#include <cstdio>
#include <raylib.h>
#include <string>

RoomLayerViewVisitor::RoomLayerViewVisitor() {
	tileSetView = TileSetView::create();
	tileSetView->setSize({"100%", "100%"});

	interactableChoose = tgui::ComboBox::create();
	interactableChoose->setDefaultText("Dialogue");

	propChoose = tgui::ComboBox::create();

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
			auto propBox = PropertiesBox::create();
			propBox->setSize("100%", "100%");
			group->add(propBox);
			propBox->addPropsJson(inter->getProps());
		}
	} else {
		group->add(tgui::Label::create("Interactables"));
	}
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_PROPS>) {
	if (tool == RoomTool::TOOL_PLACE) {
		group->add(tgui::Label::create("Props"));
		propChoose->removeAllItems();
		auto vec = Editor::instance->getProject()->getPropsNames();
		for (auto propPath : vec) {
			propChoose->addItem(GetFileNameWithoutExt(propPath.c_str()),
								propPath);
		}
		propChoose->setSelectedItemByIndex(0);

		group->add(propChoose);
	}
}
