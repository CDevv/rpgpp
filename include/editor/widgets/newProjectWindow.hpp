#ifndef _RPGPP_NEWPROJECTWINDOW_H
#define _RPGPP_NEWPROJECTWINDOW_H

#include "TGUI/Backend/raylib.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "widgets/fileChooser.hpp"

class NewProjectWindow {
  public:
	tgui::ChildWindow::Ptr window;
	tgui::EditBox::Ptr titleField;
	FileChooser::Ptr fileField;
	tgui::Button::Ptr confirmButton;
	tgui::Button::Ptr cancelButton;
	tgui::Label::Ptr fileLabel;

	typedef std::shared_ptr<NewProjectWindow> Ptr;
	typedef std::shared_ptr<const NewProjectWindow> ConstPtr;

	NewProjectWindow(const char *typeName = "NewProjectWindow",
					 bool initRenderer = true);

	static NewProjectWindow::Ptr create();
	static NewProjectWindow::Ptr create(const tgui::String &title);

	void init(tgui::Gui *gui);
	void setSize(const tgui::Layout2d &size);

	void updateSize(const tgui::Layout2d &size);
	void setFieldTitle(const tgui::String &title);
	void setFileFieldTitle(const tgui::String &title);
	void setPathFilters(
		std::vector<std::pair<tgui::String, std::vector<tgui::String>>>
			pathFilters);

  private:
	static const int FIELD_H = 24;
	static const int BUTTON_W = 100;
	static const int BUTTON_H = 24;
	static const int PADDING = 8;
};

#endif
