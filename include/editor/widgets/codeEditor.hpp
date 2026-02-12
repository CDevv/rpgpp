
#ifndef RPGPP_CODEEDITOR_H
#define RPGPP_CODEEDITOR_H
#include "TGUI/Text.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widgets/TextArea.hpp"
#include <string>


class CodeEditor : public tgui::TextArea {
  public:
	static Ptr create();

	void keyPressed(const tgui::Event::KeyEvent &event) override;
	void draw(tgui::BackendRenderTarget &target, tgui::RenderStates states) const override;

  private:
		tgui::Text constructText(const tgui::String&, tgui::Vector2f position) const;
		tgui::Text constructText(const tgui::String& text) const;
		bool canGainFocus() const;
};

#endif // RPGPP_CODEEDITOR_H
