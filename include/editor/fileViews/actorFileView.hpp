#ifndef RPGPP_ACTORFILEVIEW_H
#define RPGPP_ACTORFILEVIEW_H

#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/CheckBox.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "fileViews/fileView.hpp"
#include "views/actorView.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
class ActorFileView : public FileView {
  private:
	FileField::Ptr tileSetField;
	tgui::Button::Ptr playPauseButton;
	tgui::ComboBox::Ptr directionSelect;
	BoolField::Ptr isNonIdle;

	static const int RIGHT_PANEL_W = 300;

	ActorView::Ptr actorView{};

  public:
	ActorFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
	void updateAnimationState();
};

#endif
