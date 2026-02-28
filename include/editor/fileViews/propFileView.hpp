#ifndef _RPGPP_FILEVIEWS_PROPFILEVIEW_H
#define _RPGPP_FILEVIEWS_PROPFILEVIEW_H

#include "fileView.hpp"
#include "variant.hpp"
#include "views/propView.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/rectangleField.hpp"

class PropFileView : public FileView {
  public:
	PropFileView();
	// ~PropFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;

  private:
	void handleModePress(tgui::Vector2f pos);
	PropView::Ptr propView;
	static const int RIGHT_PANEL_W = 300;

	BoolField::Ptr hasInteractableField;
	FileField::Ptr propImageField;
	RectangleField::Ptr atlasRectField;
	RectangleField::Ptr collisionsField;
};

#endif
