#ifndef _RPGPP_INTERFACEVIEW_H
#define _RPGPP_INTERFACEVIEW_H

#include <raylib.h>

#include <memory>
#include <string>
#include <vector>

#include "gamedata.hpp"
#include "saveable.hpp"
#include "uiElement.hpp"

class InterfaceView : public ISaveable {
private:
	Rectangle rect;
	std::map<std::string, std::unique_ptr<UIElement>> elements;
	UIElement *focused = nullptr;

public:
	InterfaceView();
	explicit InterfaceView(Rectangle rect);

	InterfaceView(const std::string &filePath);
	nlohmann::json dumpJson();

	InterfaceView(InterfaceViewBin &bin);

	void onNotify(Event event);
	bool elementExists(const std::string &title);
	void addElement(const std::string &title, UIElement *element);
	void addElement(const std::string &title, std::unique_ptr<UIElement> element);
	void removeElement(const std::string &title);
	UIElement *getElement(const std::string &title);
	void renameElement(const std::string &title, const std::string &newTitle);
	void changeFocusedElement(const std::string &title);
	void update() const;
	void draw() const;
};

#endif
