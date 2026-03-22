#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "bindTranslation.hpp"
#include "raylib.h"
#include "services/translationService.hpp"
#include <TGUI/TGUI.hpp>
#include <widgets/hotkeyModifier.hpp>
// this is the most unholy shit i've wrote
const KeyboardKey tguiToRaylibKey(tguiKey k) {
	switch (k) {
	case tguiKey::A:
		return KEY_A;
	case tguiKey::B:
		return KEY_B;
	case tguiKey::C:
		return KEY_C;
	case tguiKey::D:
		return KEY_D;
	case tguiKey::E:
		return KEY_E;
	case tguiKey::F:
		return KEY_F;
	case tguiKey::G:
		return KEY_G;
	case tguiKey::H:
		return KEY_H;
	case tguiKey::I:
		return KEY_I;
	case tguiKey::J:
		return KEY_J;
	case tguiKey::K:
		return KEY_K;
	case tguiKey::L:
		return KEY_L;
	case tguiKey::M:
		return KEY_M;
	case tguiKey::N:
		return KEY_N;
	case tguiKey::O:
		return KEY_O;
	case tguiKey::P:
		return KEY_P;
	case tguiKey::Q:
		return KEY_Q;
	case tguiKey::R:
		return KEY_R;
	case tguiKey::S:
		return KEY_S;
	case tguiKey::T:
		return KEY_T;
	case tguiKey::U:
		return KEY_U;
	case tguiKey::V:
		return KEY_V;
	case tguiKey::W:
		return KEY_W;
	case tguiKey::X:
		return KEY_X;
	case tguiKey::Y:
		return KEY_Y;
	case tguiKey::Z:
		return KEY_Z;
	case tguiKey::Num0:
		return KEY_ZERO;
	case tguiKey::Num1:
		return KEY_ONE;
	case tguiKey::Num2:
		return KEY_TWO;
	case tguiKey::Num3:
		return KEY_THREE;
	case tguiKey::Num4:
		return KEY_FOUR;
	case tguiKey::Num5:
		return KEY_FIVE;
	case tguiKey::Num6:
		return KEY_SIX;
	case tguiKey::Num7:
		return KEY_SEVEN;
	case tguiKey::Num8:
		return KEY_EIGHT;
	case tguiKey::Num9:
		return KEY_NINE;
	case tguiKey::Escape:
		return KEY_ESCAPE;
	case tguiKey::LControl:
		return KEY_LEFT_CONTROL;
	case tguiKey::LShift:
		return KEY_LEFT_SHIFT;
	case tguiKey::LAlt:
		return KEY_LEFT_ALT;
	case tguiKey::LSystem:
		return KEY_LEFT_SUPER;
	case tguiKey::RControl:
		return KEY_RIGHT_CONTROL;
	case tguiKey::RShift:
		return KEY_RIGHT_SHIFT;
	case tguiKey::RAlt:
		return KEY_RIGHT_ALT;
	case tguiKey::RSystem:
		return KEY_RIGHT_SUPER;
	case tguiKey::Menu:
		return KEY_KB_MENU;
	case tguiKey::LBracket:
		return KEY_LEFT_BRACKET;
	case tguiKey::RBracket:
		return KEY_RIGHT_BRACKET;
	case tguiKey::Semicolon:
		return KEY_SEMICOLON;
	case tguiKey::Comma:
		return KEY_COMMA;
	case tguiKey::Period:
		return KEY_PERIOD;
	case tguiKey::Quote:
		return KEY_APOSTROPHE;
	case tguiKey::Slash:
		return KEY_SLASH;
	case tguiKey::Backslash:
		return KEY_BACKSLASH;
	case tguiKey::Equal:
		return KEY_EQUAL;
	case tguiKey::Minus:
		return KEY_MINUS;
	case tguiKey::Space:
		return KEY_SPACE;
	case tguiKey::Enter:
		return KEY_ENTER;
	case tguiKey::Backspace:
		return KEY_BACKSPACE;
	case tguiKey::Tab:
		return KEY_TAB;
	case tguiKey::PageUp:
		return KEY_PAGE_UP;
	case tguiKey::PageDown:
		return KEY_PAGE_DOWN;
	case tguiKey::End:
		return KEY_END;
	case tguiKey::Home:
		return KEY_HOME;
	case tguiKey::Insert:
		return KEY_INSERT;
	case tguiKey::Delete:
		return KEY_DELETE;
	case tguiKey::Add:
		return KEY_KP_ADD;
	case tguiKey::Subtract:
		return KEY_KP_SUBTRACT;
	case tguiKey::Multiply:
		return KEY_KP_MULTIPLY;
	case tguiKey::Divide:
		return KEY_KP_DIVIDE;
	case tguiKey::Left:
		return KEY_LEFT;
	case tguiKey::Right:
		return KEY_RIGHT;
	case tguiKey::Up:
		return KEY_UP;
	case tguiKey::Down:
		return KEY_DOWN;
	case tguiKey::Numpad0:
		return KEY_KP_0;
	case tguiKey::Numpad1:
		return KEY_KP_1;
	case tguiKey::Numpad2:
		return KEY_KP_2;
	case tguiKey::Numpad3:
		return KEY_KP_3;
	case tguiKey::Numpad4:
		return KEY_KP_4;
	case tguiKey::Numpad5:
		return KEY_KP_5;
	case tguiKey::Numpad6:
		return KEY_KP_6;
	case tguiKey::Numpad7:
		return KEY_KP_7;
	case tguiKey::Numpad8:
		return KEY_KP_8;
	case tguiKey::Numpad9:
		return KEY_KP_9;
	case tguiKey::F1:
		return KEY_F1;
	case tguiKey::F2:
		return KEY_F2;
	case tguiKey::F3:
		return KEY_F3;
	case tguiKey::F4:
		return KEY_F4;
	case tguiKey::F5:
		return KEY_F5;
	case tguiKey::F6:
		return KEY_F6;
	case tguiKey::F7:
		return KEY_F7;
	case tguiKey::F8:
		return KEY_F8;
	case tguiKey::F9:
		return KEY_F9;
	case tguiKey::F10:
		return KEY_F10;
	case tguiKey::F11:
		return KEY_F11;
	case tguiKey::F12:
		return KEY_F12;
	case tguiKey::Tilde:
	case tguiKey::F13:
	case tguiKey::F14:
	case tguiKey::F15:
	case tguiKey::Pause:
	case tguiKey::Unknown:
		return KEY_NULL;
	}
}
const std::string keyboardKeyToName(KeyboardKey k) {
	switch (k) {
	case KEY_NULL:
		return "Unknown";
	case KEY_APOSTROPHE:
		return "'";
	case KEY_COMMA:
		return ",";
	case KEY_MINUS:
		return "-";
	case KEY_PERIOD:
		return ".";
	case KEY_SLASH:
		return "/";
	case KEY_ZERO:
		return "0";
	case KEY_ONE:
		return "1";
	case KEY_TWO:
		return "2";
	case KEY_THREE:
		return "3";
	case KEY_FOUR:
		return "4";
	case KEY_FIVE:
		return "5";
	case KEY_SIX:
		return "6";
	case KEY_SEVEN:
		return "7";
	case KEY_EIGHT:
		return "8";
	case KEY_NINE:
		return "9";
	case KEY_SEMICOLON:
		return ";";
	case KEY_EQUAL:
		return "=";
	case KEY_A:
		return "A";
	case KEY_B:
		return "B";
	case KEY_C:
		return "C";
	case KEY_D:
		return "D";
	case KEY_E:
		return "E";
	case KEY_F:
		return "F";
	case KEY_G:
		return "G";
	case KEY_H:
		return "H";
	case KEY_I:
		return "I";
	case KEY_J:
		return "J";
	case KEY_K:
		return "K";
	case KEY_L:
		return "L";
	case KEY_M:
		return "M";
	case KEY_N:
		return "N";
	case KEY_O:
		return "O";
	case KEY_P:
		return "P";
	case KEY_Q:
		return "Q";
	case KEY_R:
		return "R";
	case KEY_S:
		return "S";
	case KEY_T:
		return "T";
	case KEY_U:
		return "U";
	case KEY_V:
		return "V";
	case KEY_W:
		return "W";
	case KEY_X:
		return "X";
	case KEY_Y:
		return "Y";
	case KEY_Z:
		return "Z";
	case KEY_LEFT_BRACKET:
		return "[";
	case KEY_BACKSLASH:
		return "\\";
	case KEY_RIGHT_BRACKET:
		return "]";
	case KEY_SPACE:
		return "Space";
	case KEY_ESCAPE:
		return "Esc";
	case KEY_ENTER:
		return "Enter";
	case KEY_TAB:
		return "Tab";
	case KEY_BACKSPACE:
		return "Backspace";
	case KEY_INSERT:
		return "Ins";
	case KEY_DELETE:
		return "Del";
	case KEY_RIGHT:
		return "Right";
	case KEY_LEFT:
		return "Left";
	case KEY_DOWN:
		return "Down";
	case KEY_UP:
		return "Up";
	case KEY_PAGE_UP:
		return "PgUp";
	case KEY_PAGE_DOWN:
		return "PgDn";
	case KEY_HOME:
		return "Home";
	case KEY_END:
		return "End";
	case KEY_F1:
		return "F1";
	case KEY_F2:
		return "F2";
	case KEY_F3:
		return "F3";
	case KEY_F4:
		return "F4";
	case KEY_F5:
		return "F5";
	case KEY_F6:
		return "F6";
	case KEY_F7:
		return "F7";
	case KEY_F8:
		return "F8";
	case KEY_F9:
		return "F9";
	case KEY_F10:
		return "F10";
	case KEY_F11:
		return "F11";
	case KEY_F12:
		return "F12";
	case KEY_LEFT_SHIFT:
		return "Shift";
	case KEY_LEFT_CONTROL:
		return "Control";
	case KEY_LEFT_ALT:
		return "Alt";
	case KEY_LEFT_SUPER:
		return "Super";
	case KEY_RIGHT_SHIFT:
		return "RightShift";
	case KEY_RIGHT_CONTROL:
		return "RightControl";
	case KEY_RIGHT_ALT:
		return "RightAlt";
	case KEY_RIGHT_SUPER:
		return "RightSuper";
	case KEY_KB_MENU:
		return "KBMenu";
	case KEY_KP_0:
		return "Keypad0";
	case KEY_KP_1:
		return "Keypad1";
	case KEY_KP_2:
		return "Keypad2";
	case KEY_KP_3:
		return "Keypad3";
	case KEY_KP_4:
		return "Keypad4";
	case KEY_KP_5:
		return "Keypad5";
	case KEY_KP_6:
		return "Keypad6";
	case KEY_KP_7:
		return "Keypad7";
	case KEY_KP_8:
		return "Keypad8";
	case KEY_KP_9:
		return "Keypad9";
	case KEY_KP_DIVIDE:
		return "Keypad /";
	case KEY_KP_MULTIPLY:
		return "Keypad *";
	case KEY_KP_SUBTRACT:
		return "Keypad -";
	case KEY_KP_ADD:
		return "Keypad +";
	}
}

HotkeyModifier::HotkeyModifier(const char *typeName, bool initRenderer)
	: tgui::SubwidgetContainer(typeName, initRenderer) {
	this->button = tgui::Button::create();
	this->button->setSize({"100%", "100%"});
	this->button->onClick([this]() {
		TranslationService &ts = Editor::instance->getTranslations();
		if (modifingState == State::DEFAULT) {
			modifingState = State::START_EDITING;
			this->button->setText(
				ts.getKey("widget.hotkey_modifier.listening"));
		}
	});
	m_container->add(this->button);
}

HotkeyModifier::Ptr HotkeyModifier::create() {
	return std::make_shared<HotkeyModifier>();
}

HotkeyModifier::Ptr HotkeyModifier::copy(HotkeyModifier::ConstPtr widget) {
	if (widget)
		return std::static_pointer_cast<HotkeyModifier>(widget->clone());
	else
		return nullptr;
}

tgui::Widget::Ptr HotkeyModifier::clone() const {
	return std::make_shared<HotkeyModifier>(*this);
}

void HotkeyModifier::setKey(const std::string &id, KeyboardKey key,
							bool isShift, bool isCtrl, bool isAlt, bool isSuper,
							bool override) {
	if (modifingState == State::DEFAULT && !override)
		return;
	keys.clear();
	if (isSuper) {
		keys.push_back(KEY_LEFT_SUPER);
	}
	if (isCtrl) {
		keys.push_back(KEY_LEFT_CONTROL);
	}
	if (isShift) {
		keys.push_back(KEY_LEFT_SHIFT);
	}
	if (isAlt) {
		keys.push_back(KEY_LEFT_ALT);
	}
	keys.push_back(key);
	std::string label{};
	for (auto i = keys.begin(); i != keys.end(); ++i) {
		if (i != keys.begin())
			label += "+";
		label += keyboardKeyToName(*i);
	}
	this->button->setText(label);

	hk = {
		.ctrl = isCtrl,
		.shift = isShift,
		.alt = isAlt,
		.super = isSuper,
		.key = key,
	};
	this->id = id;
}

void HotkeyModifier::keyPressed(const tgui::Event::KeyEvent &event) {
	tgui::SubwidgetContainer::keyPressed(event);
	setKey(this->id, tguiToRaylibKey(event.code), event.shift, event.control,
		   event.alt, event.system);
	modifingState = State::DEFAULT;
	onChange.emit(this, this->id, hk);
}
