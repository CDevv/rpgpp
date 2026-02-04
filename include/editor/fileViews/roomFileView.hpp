#ifndef _RPGPP_ROOMFILEVIEW_H
#define _RPGPP_ROOMFILEVIEW_H

#include "fileView.hpp"
#include "tileSetView.hpp"
#include "widgets/fileField.hpp"
#include "widgets/intField.hpp"
#include "widgets/roomView.hpp"

class RoomFileView : public FileView {
  private:
	RoomView::Ptr roomView;
	TileSetView::Ptr tileSetView;
	IntField::Ptr widthField;
	IntField::Ptr heightField;
	FileField::Ptr tileSetField;

  public:
	RoomFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif