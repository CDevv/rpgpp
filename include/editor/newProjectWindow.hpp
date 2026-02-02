#ifndef _RPGPP_NEWPROJECTWINDOW_H
#define _RPGPP_NEWPROJECTWINDOW_H

#include "TGUI/Renderers/ChildWindowRenderer.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include <TGUI/AllWidgets.hpp>

/*
class NewProjectWindowRenderer : public tgui::ChildWindowRenderer {
  public:
	using tgui::ChildWindowRenderer::ChildWindowRenderer;
};

class NewProjectWindow : public tgui::ChildWindow {
  public:
	typedef std::shared_ptr<NewProjectWindow> Ptr;
	typedef std::shared_ptr<const NewProjectWindow> ConstPtr;

	NewProjectWindow(const char *typeName = "NewProjectWindow",
					 bool initRenderer = true);

	static NewProjectWindow::Ptr create();

	static NewProjectWindow::Ptr copy(NewProjectWindow::ConstPtr widget);

	NewProjectWindowRenderer *getSharedRenderer() override;

	const NewProjectWindowRenderer *getSharedRenderer() const override;

	NewProjectWindowRenderer *getRenderer() override;

	void rendererChanged(const tgui::String &property) override;

	void draw(tgui::BackendRenderTarget &target,
			  tgui::RenderStates states) const override;

  protected:
	Widget::Ptr clone() const override;
};
*/

class NewProjectWindow
	: public tgui::ChildWindow /* base type depends on approach */
{
  public:
	// Add a Ptr typedef so that "NewProjectWindow::Ptr" has the correct type
	typedef std::shared_ptr<NewProjectWindow> Ptr;
	typedef std::shared_ptr<const NewProjectWindow> ConstPtr;

	// Add a constructor that sets the type name. This is the string that would
	// be returned when calling getWidgetType(). We assume that we can keep the
	// renderer of the base class in this code. See the "Changing renderer"
	// section later for an explanation on how to make changes to the renderer
	// as well.
	NewProjectWindow(const char *typeName = "NewProjectWindow",
					 bool initRenderer = true)
		: tgui::ChildWindow(typeName,
							initRenderer) /* base type depends on approach */
	{}

	// Copy constructors, assignment operators and destructor can be added if
	// required for the members that you add to the custom widget. These
	// functions are not required by default.

	// Every widget needs a static create function which creates the widget and
	// returns a smart pointer to it. This function can have any parameters you
	// want.
	static NewProjectWindow::Ptr create() {
		return std::make_shared<NewProjectWindow>();
	}

	// Every widget has a static method to copy it
	static NewProjectWindow::Ptr copy(NewProjectWindow::ConstPtr widget) {
		if (widget)
			return std::static_pointer_cast<NewProjectWindow>(widget->clone());
		else
			return nullptr;
	}

  protected:
	// Every widget needs a method to copy it
	Widget::Ptr clone() const override {
		return std::make_shared<NewProjectWindow>(*this);
	}
};

#endif