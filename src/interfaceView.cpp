#include "interfaceView.hpp"

#include <memory>
#include <string>
#include <utility>

#include "actor.hpp"
#include "gamedata.hpp"
#include "interfaceElementFactory.hpp"
#include "raylib.h"
#include "uiElement.hpp"

InterfaceView::InterfaceView() : rect(Rectangle{}) {}

InterfaceView::InterfaceView(Rectangle rect) {
	this->rect = rect;
	this->elements = std::map<std::string, std::unique_ptr<UIElement>>{};
}

InterfaceView::InterfaceView(const std::string &filePath) {
	std::string fileText = LoadFileText(filePath.c_str());

	auto j = json::parse(fileText);
	this->rect = {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
	for (auto &item : j.at("elements").items()) {
		auto obj = item.value();
		auto element = constructElement(obj.at("type"));
		element->fromJson(obj.at("props"));
		addElement(item.key(), std::move(element));
	}
}

InterfaceView::InterfaceView(InterfaceViewBin &bin) {
	for (auto &[title, elementBin] : bin.elements) {
		auto element = constructElement(elementBin.type);
		element->fromBin(elementBin);
		addElement(title, std::move(element));
	}
}

nlohmann::json InterfaceView::dumpJson() {
	auto j = json::object();
	j["elements"] = json::object();
	for (auto &&[title, element] : elements) {
		auto obj = json::object();
		obj["type"] = element->getType();
		obj["props"] = element->dumpJson();

		j["elements"][title] = obj;
	}

	return j;
}

bool InterfaceView::elementExists(const std::string &title) { return (elements.count(title) > 0); }

void InterfaceView::addElement(const std::string &title, UIElement *element) {
	auto ptr = std::unique_ptr<UIElement>{};
	ptr.reset(element);
	this->elements[title] = std::move(ptr);
}

void InterfaceView::addElement(const std::string &title, std::unique_ptr<UIElement> element) {
	this->elements[title] = std::move(element);
}

void InterfaceView::removeElement(const std::string &title) { this->elements.erase(title); }

UIElement *InterfaceView::getElement(const std::string &title) { return this->elements.at(title).get(); }

void InterfaceView::renameElement(const std::string &title, const std::string &newTitle) {
	auto node = elements.extract(title);
	node.key() = newTitle;
	elements.insert(std::move(node));
}

void InterfaceView::changeFocusedElement(const std::string &title) {
	if (elementExists(title)) {
		if (elements[title]->isFocusable()) {
			if (focused != nullptr) {
				focused->invokeCallback(CALLBACK_UNFOCUSED);
			}
			focused = elements[title].get();
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
	for (auto &&[key, i] : elements) {
		i->update();
	}
}

void InterfaceView::draw() const {
	for (auto &&[key, i] : elements) {
		i->draw();
	}
}
