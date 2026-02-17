#ifndef _RPGPP_ROOMFILEVIEW_H
#define _RPGPP_ROOMFILEVIEW_H

#include "fileView.hpp"
#include "roomLayerViewVisitor.hpp"
#include "roomViewModesHandler.hpp"
#include "views/tileSetView.hpp"
#include "widgets/fileField.hpp"
#include "widgets/intField.hpp"
#include "views/roomView.hpp"
#include <memory>

class RoomFileView : public FileView {
  private:
	static const int RIGHT_PANEL_W = 300;
	static const int LAYER_CHOOSE_H = 32;
	static const int TOOLBOX_H = LAYER_CHOOSE_H;
	static const int ROOM_LAYER_W = RIGHT_PANEL_W;
	static const int ROOM_LAYER_H = RIGHT_PANEL_W;

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
