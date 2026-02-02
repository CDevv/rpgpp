#ifndef _RPGPP_PROPERTIESBOX_H
#define _RPGPP_PROPERTIESBOX_H

#include "TGUI/String.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "widgets/fileField.hpp"
#include "widgets/intField.hpp"
#include <functional>

class PropertiesBox : public tgui::ChildWindow {
  protected:
	tgui::GrowVerticalLayout::Ptr layout;
	tgui::Widget::Ptr clone() const override;

  public:
	typedef std::shared_ptr<PropertiesBox> Ptr;
	typedef std::shared_ptr<const PropertiesBox> ConstPtr;

	PropertiesBox(const char *typeName = "PropertiesBox",
				  bool initRenderer = true);

	static PropertiesBox::Ptr create();
	static PropertiesBox::Ptr copy(PropertiesBox::ConstPtr widget);

	void draw(tgui::BackendRenderTarget &target,
			  tgui::RenderStates states) const override;

	void addToggleField(const tgui::String &title);
	void addIntField(const tgui::String &title, int initialValue,
					 std::function<void(float)> callback);
	void addIntField(IntField::Ptr field);
	void addFileField(FileField::Ptr field);
	void addButton(const tgui::String &title, std::function<void()> callback);
};

#endif