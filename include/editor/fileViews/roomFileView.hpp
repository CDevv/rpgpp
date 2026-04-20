#ifndef _RPGPP_ROOMFILEVIEW_H
#define _RPGPP_ROOMFILEVIEW_H

#include <TGUI/Widgets/ComboBox.hpp>
#include <memory>

#include "fileView.hpp"
#include "roomLayerViewVisitor.hpp"
#include "roomViewModesHandler.hpp"
#include "views/roomView.hpp"
#include "views/tileSetView.hpp"
#include "views/worldView.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/toolbox.hpp"

class RoomFileView : public FileView {
private:
	static const int RIGHT_PANEL_W = 300;
	static const int LAYER_CHOOSE_H = 32;
	static const int TOOLBOX_H = LAYER_CHOOSE_H;
	static const int ROOM_LAYER_W = RIGHT_PANEL_W;
	static const int ROOM_LAYER_H = RIGHT_PANEL_W;

	RoomLayerViewVisitor layerVisitor;
	tgui::ComboBox::Ptr layerChoose;
	RoomView::Ptr roomView;
	TileSetView::Ptr tileSetView;
	IntField::Ptr widthField;
	IntField::Ptr heightField;
	FileField::Ptr tileSetField;
	FileField::Ptr musicFileField;

	void setRoomTool(ToolboxItem<RoomTool> tool);
	std::vector<std::string> hotkeyEntries;

public:
	std::unique_ptr<RoomViewModesHandler> modesHandler;
	RoomFileView();
	~RoomFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif
