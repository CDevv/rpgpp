#ifndef RPGPP_ACTORFILEVIEW_H
#define RPGPP_ACTORFILEVIEW_H

#include "fileViews/fileView.hpp"
#include "views/actorView.hpp"
#include "widgets/frameEditor.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/rectangleField.hpp"

class ActorFileView : public FileView {
  private:
	FrameEditor::Ptr frameEditor;
	
	FileField::Ptr tileSetField;
	RectangleField::Ptr collisionField;
	ActorView::Ptr actorView;

  public:
	ActorFileView();

	ActorView::Ptr getActorView() const { return actorView; }

	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif
