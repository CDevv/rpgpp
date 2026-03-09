#ifndef _RPGPP_FILECHOOSER_H
#define _RPGPP_FILECHOOSER_H

#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/EditBox.hpp"

class FileChooser : public tgui::SubwidgetContainer {
  private:
	tgui::String chosenPath;
	bool selectingDirectory = false;
	static const int PADDING = 4;

  public:
	tgui::EditBox::Ptr chosenPathLabel;
	tgui::BitmapButton::Ptr iconButton;
	std::vector<std::pair<tgui::String, std::vector<tgui::String>>> pathFilters;

	typedef std::shared_ptr<FileChooser> Ptr;
	typedef std::shared_ptr<const FileChooser> ConstPtr;

	FileChooser(const char *typeName = "FileChooser", bool initRenderer = true);

	static FileChooser::Ptr create();

	static FileChooser::Ptr copy(FileChooser::ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;

	tgui::String &getChosenPath();

	void setSelectingDirectory(bool selectingDirectory);

  private:
	void updateSize();

  protected:
	Widget::Ptr clone() const override;
};

#endif
