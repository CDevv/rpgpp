#ifndef _RPGPP_FILEVIEWS_INTERACTABLEFILEVIEW_H
#define _RPGPP_FILEVIEWS_INTERACTABLEFILEVIEW_H

#include "fileView.hpp"
#include "variant.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/textField.hpp"

class InteractableFileView : public FileView {
private:
	static const int RIGHT_PANEL_W = 300;
	TextField::Ptr nameField;
	FileField::Ptr scriptField;
	BoolField::Ptr onTouchField;
	PropertiesBox::Ptr interProps;

public:
	InteractableFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif