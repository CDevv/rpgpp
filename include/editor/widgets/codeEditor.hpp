
#ifndef RPGPP_CODEEDITOR_H
#define RPGPP_CODEEDITOR_H
#include "TGUI/Text.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widgets/TextArea.hpp"
#include <string>

using namespace tgui;

class CodeEditor : public tgui::TextArea {
  public:
	static Ptr create();

	void keyPressed(const Event::KeyEvent &event) override;

	private:
		Text createText(const tgui::String& text, const tgui::Vector2f& position);
};

#endif // RPGPP_CODEEDITOR_H
