#ifndef _RPGPP_FILETAB_H
#define _RPGPP_FILETAB_H

#include "TGUI/Signal.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Tabs.hpp"
#include "fileTabRenderer.hpp"
#include <cstddef>

class FileTab : public tgui::Tabs {
  private:
    mutable bool isHovering = false;
    mutable bool isDragging = false;
    mutable tgui::Vector2f startMousePos;
    mutable tgui::Vector2f endMousePos;
    static const int MARGIN_LR = 8;
    static const int CLOSE_BUTTON_SIZE = 12;
    constexpr static const tgui::Vector2f BUFFER_BEFORE_TAB_MOVE = {10, 10};

  public:
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
	void mouseMoved(tgui::Vector2f pos) override;
	void leftMouseReleased(tgui::Vector2f pos) override;

	bool select(std::size_t i);

	size_t addFileTab(const std::string &path, const std::string &fileName);

  protected:
	Widget::Ptr clone() const override;
};

#endif
