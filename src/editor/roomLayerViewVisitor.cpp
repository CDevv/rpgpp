#include "roomLayerViewVisitor.hpp"

#include <raylib.h>

#include <memory>
#include <string>

#include "TGUI/Widgets/CheckBox.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "actor.hpp"
#include "editor.hpp"
#include "interactable.hpp"
#include "services/fileSystemService.hpp"
#include "views/worldView.hpp"
#include "widgets/propertiesBox.hpp"

RoomLayerViewVisitor::RoomLayerViewVisitor() {
	tileSetView = TileSetView::create();
	tileSetView->setSize({"100%", "100%"});

	interactableChoose = tgui::ComboBox::create();
	interactableChoose->setPosition(8, 32);
	interactableChoose->setDefaultText("Dialogue");

	propChoose = tgui::ComboBox::create();
	propChoose->onItemSelect([this](int index) {
		auto id = propChoose->getIdByIndex(index);
		Prop p(id.toStdString());

		if (IsTextureValid(propTexture)) {
			UnloadTexture(propTexture);
		}

		propTexture = p.getTexture();
	});

	actorNameInput = tgui::EditBox::create();
	actorNameInput->setPosition(0, 32);

	actorChoose = tgui::ComboBox::create();
	actorChoose->setPosition(0, 64);
	actorChoose->onItemSelect([this](int index) {
		auto id = actorChoose->getIdByIndex(index);
		std::unique_ptr<Actor> a = std::make_unique<Actor>(id.toStdString());

		if (IsTextureValid(actorTexture)) {
			UnloadTexture(actorTexture);
		}

		actorTexture = a->getTileSet().getTexture();
		chosenActor = std::move(a);
	});

	auto map = Editor::instance->getProject()->getInteractableNames();
	interactableChoose->setSelectedItemByIndex(0);
}

RoomLayerViewVisitor::~RoomLayerViewVisitor() {
	if (IsTextureValid(propTexture)) {
		UnloadTexture(propTexture);
	}
}

void RoomLayerViewVisitor::updateInteractableChoose() {
	interactableChoose->onItemSelect.disconnectAll();
	interactableChoose->removeAllItems();
	auto map = Editor::instance->getProject()->getInteractableNames();
	for (auto &[key, val] : map) {
		interactableChoose->addItem(val, key.c_str());
	}
	interactableChoose->setSelectedItemByIndex(0);
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_TILES>) {
	isAvailable = true;
	group->add(tileSetView);
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_COLLISION>) {
	isAvailable = true;
	group->add(tgui::Label::create("Collision Layer"));
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_INTERACTABLES>) {
	isAvailable = true;
	updateInteractableChoose();

	if (tool == RoomTool::TOOL_PLACE) {
		group->add(tgui::Label::create("Interactables"));

		group->add(interactableChoose);
	} else if (tool == RoomTool::TOOL_EDIT) {
		if (inter == nullptr) {
			group->add(tgui::Label::create("Interactables"));
		} else {
			auto interactableNames = Editor::instance->getProject()->getInteractableNames();
			std::unique_ptr<Interactable> typeInter;
			for (auto &[key, val] : interactableNames) {
				if (std::string(GetFileNameWithoutExt(key.c_str())) == inter->getType()) {
					typeInter = std::make_unique<Interactable>(key);

					// Add missing props.
					for (auto item : typeInter->getProps().items()) {
						if (!inter->getProps().contains(item.key())) {
							inter->getProps().push_back({item.key(), item.value()});
						}
					}

					// Remove non-existent props.
					auto *ptr = inter->getPropsPtr();
					for (auto item : inter->getPropsPtr()->items()) {
						if (!typeInter->getProps().contains(item.key())) {
							inter->getProps().erase(item.key());
						}
					}

					break;
				}
			}

			auto onTouchCheck = tgui::CheckBox::create("Interact on touch?");
			onTouchCheck->setSize(24, 24);
			onTouchCheck->setPosition(8, 8);
			onTouchCheck->setChecked(inter->isOnTouch());
			onTouchCheck->onCheck([this](bool value) { inter->setOnTouch(value); });
			group->add(onTouchCheck);

			auto propBox = PropertiesBox::create();
			propBox->setSize("100%", "100%");
			propBox->setPosition(0, 36);
			group->add(propBox);
			propBox->addPropsJson(inter->getProps());
		}
	} else {
		group->add(tgui::Label::create("Interactables"));
	}
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_PROPS>) {
	isAvailable = true;
	updateInteractableChoose();

	if (tool == RoomTool::TOOL_PLACE) {
		group->add(tgui::Label::create("Props"));
		propChoose->removeAllItems();
		auto vec = Editor::instance->getProject()->getPropsNames();
		for (auto propPath : vec) {
			propChoose->addItem(GetFileNameWithoutExt(propPath.c_str()), propPath);
		}

		isAvailable = (!vec.empty());

		propChoose->setSelectedItemByIndex(0);

		if (isAvailable) {
			Prop p(propChoose->getSelectedItemId().toStdString());
			propTexture = p.getTexture();
		}

		group->add(propChoose);
	} else if (tool == RoomTool::TOOL_ERASE) {
		group->add(tgui::Label::create("Erase a Prop.."));
	} else if (tool == RoomTool::TOOL_EDIT) {
		if (prop == nullptr) {
			group->add(tgui::Label::create("No valid Prop."));
		} else {
			if (prop->getHasInteractable()) {
				auto propBox = PropertiesBox::create();
				propBox->setSize("100%", "100%");
				group->add(propBox);
				propBox->addPropsJson(prop->getInteractable()->getProps());
			} else {
				group->add(tgui::Label::create("This Prop has no Interactable"));
			}
		}
	}
}

void RoomLayerViewVisitor::operator()(enum_v<RoomLayer::LAYER_ACTORS>) {
	updateInteractableChoose();

	if (tool == RoomTool::TOOL_PLACE) {
		group->add(tgui::Label::create("Actors"));

		group->add(actorNameInput);

		actorChoose->removeAllItems();
		auto vec = Editor::instance->getProject()->getPaths(EngineFileType::FILE_ACTOR);
		for (auto actorPath : vec) {
			actorChoose->addItem(GetFileNameWithoutExt(actorPath.c_str()), actorPath);
		}
		actorChoose->setSelectedItemByIndex(0);

		std::unique_ptr<Actor> a = std::make_unique<Actor>(actorChoose->getSelectedItemId().toStdString());

		actorTexture = a->getTileSet().getTexture();
		chosenActor = std::move(a);

		group->add(actorChoose);
	} else if (tool == RoomTool::TOOL_ERASE) {
		group->add(tgui::Label::create("Erase an Actor.."));
	} else if (tool == RoomTool::TOOL_EDIT) {
		if (actor == nullptr) {
			group->add(tgui::Label::create("Actor does not exist at this position."));
		} else {
			interactableChoose->setPosition(8, 64);

			interactableChoose->onItemSelect([this](const tgui::String &item) {
				auto id = interactableChoose->getSelectedItemId();
				printf("%s: %s \n", item.toStdString().c_str(), id.toStdString().c_str());

				actor->setInteractableFromPath(id.toStdString());
			});

			group->add(tgui::Label::create(TextFormat("Editing \"%s\"", actorName.c_str())));

			group->add(interactableChoose);

			interactableChoose->setDefaultText("");

			auto hasInteractableCheck = tgui::CheckBox::create("Has Interactable?");
			hasInteractableCheck->setSize(16, 16);
			hasInteractableCheck->setPosition(8, 32);

			auto propBox = PropertiesBox::create();
			propBox->setPosition(0, 84);
			propBox->setSize("100%", "100% - 84");
			group->add(propBox);

			if (actor->hasInteractable()) {
				auto intsMap = Editor::instance->getProject()->getInteractableNames();
				for (auto &[key, val] : intsMap) {
					std::string lowerTypeName = TextToLower(val.c_str());
					if (lowerTypeName == actor->getInteractable()->getType()) {
						interactableChoose->setSelectedItemById(key);
					}
				}
				propBox->addPropsJson(actor->getInteractable()->getProps());
				interactableChoose->setVisible(true);
				hasInteractableCheck->setChecked(true);
			} else {
				interactableChoose->setVisible(false);
				hasInteractableCheck->setChecked(false);
			}

			std::weak_ptr<PropertiesBox> weakPropsBox = propBox;
			hasInteractableCheck->onChange([this, weakPropsBox](bool value) {
				interactableChoose->setVisible(value);
				if (!weakPropsBox.expired()) {
					weakPropsBox.lock()->setVisible(value);
				}
				if (value) {
					interactableChoose->setSelectedItemByIndex(0);
					actor->setInteractableFromPath(interactableChoose->getSelectedItemId().toStdString());
					if (!weakPropsBox.expired()) {
						weakPropsBox.lock()->addPropsJson(actor->getInteractable()->getProps());
					}
				} else {
					actor->setHasInteractable(false);
				}
			});

			group->add(hasInteractableCheck);
		}
	}
}
