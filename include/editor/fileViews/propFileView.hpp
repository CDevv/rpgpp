#ifndef _RPGPP_FILEVIEWS_PROPFILEVIEW_H
#define _RPGPP_FILEVIEWS_PROPFILEVIEW_H

#include "fileView.hpp"
#include "variant.hpp"
#include "views/propPreview.hpp"
#include "views/propView.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/rectangleField.hpp"
#include "widgets/propertyFields/selectField.hpp"

class PropFileView : public FileView {
public:
	PropFileView();
	// ~PropFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;

private:
	void handleModePress(tgui::Vector2f pos);
	PropView::Ptr propView;
	PropPreview::Ptr propPreview;

	static const int RIGHT_PANEL_W = 300;

	BoolField::Ptr hasInteractableField;
	FileField::Ptr propImageField;
	SelectField::Ptr interactableTypeField;
	RectangleField::Ptr atlasRectField;
	RectangleField::Ptr collisionsField;
};

#endif
