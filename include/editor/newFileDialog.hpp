#ifndef _RPGPP_NEWFILEDIALOG_H
#define _RPGPP_NEWFILEDIALOG_H

#include "TGUI/Backend/raylib.hpp"
#include "TGUI/Renderers/ChildWindowRenderer.hpp"
#include "TGUI/String.hpp"
#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "widgets/fileChooser.hpp"
#include "widgets/fileField.hpp"

class NewFileDialogRenderer : public tgui::ChildWindowRenderer {
  public:
	using tgui::ChildWindowRenderer::ChildWindowRenderer;
};

class NewFileDialog {
  public:
	tgui::ChildWindow::Ptr window;
	tgui::EditBox::Ptr titleField;
	FileChooser::Ptr fileField;
	tgui::Button::Ptr confirmButton;
	tgui::Button::Ptr cancelButton;
	tgui::Label::Ptr fileLabel;

	typedef std::shared_ptr<NewFileDialog> Ptr;
	typedef std::shared_ptr<const NewFileDialog> ConstPtr;

	NewFileDialog(const char *typeName = "NewFileDialog",
				  bool initRenderer = true);

	static NewFileDialog::Ptr create();
	static NewFileDialog::Ptr create(const tgui::String &title);

	void init(tgui::Gui *gui);
	void setSize(const tgui::Layout2d &size);

	void updateSize(const tgui::Layout2d &size);
	void setFieldTitle(const tgui::String &title);
	void setFileFieldTitle(const tgui::String &title);
	void setPathFilters(
		std::vector<std::pair<tgui::String, std::vector<tgui::String>>>
			pathFilters);
};

#endif