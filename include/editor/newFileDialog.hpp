#ifndef _RPGPP_NEWFILEDIALOG_H
#define _RPGPP_NEWFILEDIALOG_H

#include "TGUI/Backend/raylib.hpp"
#include "TGUI/String.hpp"
#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "fileField.hpp"
#include <functional>

class NewFileDialog {
  private:
	void onConfirm();

  public:
	tgui::ChildWindow::Ptr window;
	tgui::EditBox::Ptr titleField;
	FileField::Ptr fileField;
	tgui::Button::Ptr confirmButton;
	tgui::Button::Ptr cancelButton;
	std::function<void(const std::string &title, const std::string &path)>
		callback{};
	std::function<void()> try2;

	typedef std::shared_ptr<NewFileDialog> Ptr;
	typedef std::shared_ptr<const NewFileDialog> ConstPtr;

	NewFileDialog() = default;

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