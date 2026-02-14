#ifndef _RPGPP_FILETAB_H
#define _RPGPP_FILETAB_H

#include "TGUI/Signal.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Tabs.hpp"
#include "fileTabRenderer.hpp"
#include <cstddef>

class FileTab : public tgui::Tabs {
  public:
    static const int MARGIN_LR = 8;
    static const int CLOSE_BUTTON_SIZE = 12;

	typedef std::shared_ptr<FileTab> Ptr;
	typedef std::shared_ptr<const FileTab> ConstPtr;
	tgui::SignalTyped<tgui::String> onTabClose = {"TabClose"};
	tgui::SignalTyped<tgui::String> onTabSelect = {"TabSelect"};

	FileTab(const char *typeName = "FileTab", bool initRenderer = true);

	static FileTab::Ptr create();

	static FileTab::Ptr copy(FileTab::ConstPtr widget);

	bool isMouseOnWidget(tgui::Vector2f pos) const override;

	void draw(tgui::BackendRenderTarget &target,
			  tgui::RenderStates states) const override;

	FileTabRenderer *getSharedRenderer() override;

	const FileTabRenderer *getSharedRenderer() const override;

	FileTabRenderer *getRenderer() override;

	bool leftMousePressed(tgui::Vector2f pos) override;

	bool select(std::size_t i);

	size_t addFileTab(const std::string &path, const std::string &fileName);

  private:
    mutable bool isHovering = false;

  protected:
	Widget::Ptr clone() const override;
};

#endif
