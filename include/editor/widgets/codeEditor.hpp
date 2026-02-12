
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
	void draw(BackendRenderTarget &target, RenderStates states) const override;
  private:
		Text constructText(const tgui::String&, Vector2f position) const;
		Text constructText(const tgui::String& text) const;
};

#endif // RPGPP_CODEEDITOR_H
