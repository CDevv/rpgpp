#ifndef _RPGPP_INTERFACEVIEW_H
#define _RPGPP_INTERFACEVIEW_H

#include <raylib.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "gamedata.hpp"
#include "saveable.hpp"
#include "uiElement.hpp"

class InterfaceView : public ISaveable {
private:
	Rectangle rect;
	std::multimap<int, std::unique_ptr<UIElement>, std::less<int>> elements;
	UIElement *focused = nullptr;

public:
	InterfaceView();
	explicit InterfaceView(Rectangle rect);

	InterfaceView(const std::string &filePath);
	InterfaceView(InterfaceViewBin &bin);

	nlohmann::json dumpJson();

	void onNotify(Event event);

	bool elementExists(const std::string &title);
	void addElement(const std::string &title, UIElement *element, int layer);
	void addElement(const std::string &title, std::unique_ptr<UIElement> element, int layer);
	void removeElement(const std::string &title);
	UIElement *getElement(const std::string &title);
	void renameElement(const std::string &title, const std::string &newTitle);
	void changeFocusedElement(const std::string &title);

	void update() const;
	void draw() const;
};

#endif
