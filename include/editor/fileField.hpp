#ifndef _RPGPP_FILEFIELD_H
#define _RPGPP_FILEFIELD_H

#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/Label.hpp"
#include <functional>
#include <memory>

class FileField : public tgui::SubwidgetContainer {
  private:
	tgui::String chosenPath;
	void updateSize();

  protected:
	tgui::Widget::Ptr clone() const override;

  public:
	tgui::Label::Ptr label;
	tgui::Button::Ptr value;
	std::vector<std::pair<tgui::String, std::vector<tgui::String>>> pathFilters;
	std::function<void(const tgui::String &path)> callback;

	typedef std::shared_ptr<FileField> Ptr;
	typedef std::shared_ptr<const FileField> ConstPtr;

	FileField(const char *typeName = "FileField", bool initRenderer = true);

	static FileField::Ptr create();
	static FileField::Ptr create(const tgui::String &label,
								 const tgui::String &value);
	static FileField::Ptr copy(FileField::ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;
	tgui::String &getChosenPath();
};

#endif