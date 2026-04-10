
#ifndef _RPGPP_HOTKEYMODIFIER_H
#define _RPGPP_HOTKEYMODIFIER_H

#include "TGUI/Signal.hpp"
#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "editor.hpp"
#include "raylib.h"

enum State { DEFAULT, START_EDITING, IS_EDITING };

using tguiKey = tgui::Event::KeyboardKey;

class HotkeyModifier : public tgui::SubwidgetContainer {
private:
	std::vector<KeyboardKey> keys;
	Hotkey hk;
	tgui::Button::Ptr button;
	State modifingState = DEFAULT;
	std::string id;

public:
	typedef std::shared_ptr<HotkeyModifier> Ptr;
	typedef std::shared_ptr<const HotkeyModifier> ConstPtr;
	HotkeyModifier(const char *typeName = "FileChooser", bool initRenderer = true);
	static HotkeyModifier::Ptr create();
	static HotkeyModifier::Ptr copy(HotkeyModifier::ConstPtr widget);

	void keyPressed(const tgui::Event::KeyEvent &event) override;

	void setKey(const std::string &id, KeyboardKey key, bool isShift, bool isCtrl, bool isAlt, bool isSuper,
				bool override = false);

	tgui::SignalTyped2<const std::string &, Hotkey> onChange = {"onChange"};

protected:
	Widget::Ptr clone() const override;
};

#endif
