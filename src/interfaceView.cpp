#include "interfaceView.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "actor.hpp"
#include "gamedata.hpp"
#include "interfaceElementFactory.hpp"
#include "raylib.h"
#include "uiElement.hpp"

InterfaceView::InterfaceView() : rect(Rectangle{}) {}

InterfaceView::InterfaceView(Rectangle rect) {
	this->rect = rect;
	this->elements = std::multimap<int, std::unique_ptr<UIElement>, std::less<int>>{};
}

InterfaceView::InterfaceView(const std::string &filePath) {
	std::string fileText = LoadFileText(filePath.c_str());

	auto j = json::parse(fileText);
	this->rect = {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
	for (auto &item : j.at("elements").items()) {
		auto obj = item.value();
		auto element = constructElement(obj.at("type"));
		element->fromJson(obj.at("props"));
		int layer = obj.at("layer");
		addElement(item.key(), std::move(element), layer);
	}
}

InterfaceView::InterfaceView(InterfaceViewBin &bin) {
	for (auto &[title, elementBin] : bin.elements) {
		auto element = constructElement(elementBin.type);
		element->fromBin(elementBin);
		addElement(title, std::move(element), elementBin.layer);
	}
}

nlohmann::json InterfaceView::dumpJson() {
	auto j = json::object();
	j["elements"] = json::object();
	for (auto &&[title, element] : elements) {
		auto obj = json::object();
		obj["type"] = element->getType();
		obj["layer"] = element->getLayer();
		obj["props"] = element->dumpJson();

		j["elements"][title] = obj;
	}

	return j;
}

bool InterfaceView::elementExists(const std::string &title) {
	bool res = false;
	for (auto &item : elements) {
		if (item.second->getName() == title) {
			res = true;
			break;
		}
	}
	return res;
}

void InterfaceView::addElement(const std::string &title, UIElement *element, int layer) {
	element->setName(title);
	element->setLayer(layer);
	auto ptr = std::unique_ptr<UIElement>{};
	ptr.reset(element);
	this->elements.emplace(std::make_pair(layer, std::move(ptr)));
}

void InterfaceView::addElement(const std::string &title, std::unique_ptr<UIElement> element, int layer) {
	element->setName(title);
	element->setLayer(layer);
	this->elements.emplace(std::make_pair(layer, std::move(element)));
}

void InterfaceView::removeElement(const std::string &title) {
	for (auto &item : elements) {
		if (item.second->getName() == title) {
			elements.erase(item.first);
			break;
		}
	}
}

UIElement *InterfaceView::getElement(const std::string &title) {
	UIElement *res = nullptr;
	for (auto &&item : elements) {
		if (item.second->getName() == title) {
			res = item.second.get();
			break;
		}
	}
	return res;
}

void InterfaceView::renameElement(const std::string &title, const std::string &newTitle) {
	for (auto &item : elements) {
		if (item.second->getName() == title) {
			item.second->setName(newTitle);
			break;
		}
	}
}

void InterfaceView::changeFocusedElement(const std::string &title) {
	if (elementExists(title)) {
		auto *element = getElement(title);
		if (element->isFocusable()) {
			if (focused != nullptr) {
				focused->invokeCallback(CALLBACK_UNFOCUSED);
			}
			focused = element;
			focused->invokeCallback(CALLBACK_FOCUSED);
		}
	}
}

void InterfaceView::onNotify(Event event) {
	if (focused != nullptr) {
		focused->onNotify(event);
	}
}

void InterfaceView::update() const {
	for (auto &item : elements) {
		item.second->update();
	}
}

void InterfaceView::draw() const {
	for (auto &item : elements) {
		if (item.second->isVisible()) {
			item.second->draw();
		}
	}
}
