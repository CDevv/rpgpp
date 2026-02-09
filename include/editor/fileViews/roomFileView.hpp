#ifndef _RPGPP_ROOMFILEVIEW_H
#define _RPGPP_ROOMFILEVIEW_H

#include "fileView.hpp"
#include "roomLayerViewVisitor.hpp"
#include "roomViewModesHandler.hpp"
#include "tileSetView.hpp"
#include "widgets/fileField.hpp"
#include "widgets/intField.hpp"
#include "widgets/roomView.hpp"
#include <memory>

class RoomFileView : public FileView {
  private:
	RoomLayerViewVisitor layerVisitor;
	RoomView::Ptr roomView;
	TileSetView::Ptr tileSetView;
	IntField::Ptr widthField;
	IntField::Ptr heightField;
	FileField::Ptr tileSetField;

  public:
	std::unique_ptr<RoomViewModesHandler> modesHandler;
	RoomFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif