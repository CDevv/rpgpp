#ifndef _RPGPP_VIEWS_FONTVIEW_H
#define _RPGPP_VIEWS_FONTVIEW_H

#include "saveables/fontWrapper.hpp"
#include "views/worldView.hpp"
#include <memory>

class FontView : public WorldView {
  public:
	typedef std::shared_ptr<FontView> Ptr;

	FontView();

	FontWrapper *font{nullptr};
	int baseFontSize{16};
	std::string text{"The lazy fop."};

	static Ptr create();

	void setFont(FontWrapper *wrapper);
	FontWrapper *getFont();

	void setBaseFontSize(int newSize);

	void setText(const std::string &text);
	const std::string &getText();

	void drawCanvas() override;
	void drawOverlay() override;
};

#endif