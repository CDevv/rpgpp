#ifndef _RPGPP_TOOLBOX_H
#define _RPGPP_TOOLBOX_H

#include "TGUI/Renderers/TabsRenderer.hpp"
#include "TGUI/Signal.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/CheckBox.hpp"
#include "TGUI/Widgets/Tabs.hpp"
#include "fileSystemService.hpp"
#include <vector>

enum class RoomTool { TOOL_NONE, TOOL_PLACE, TOOL_ERASE, TOOL_EDIT };

struct RoomToolboxItem {
	RoomTool type;
	tgui::Texture texture;
};

class RoomToolboxRenderer : public tgui::TabsRenderer {
	using tgui::TabsRenderer::TabsRenderer;
};

class RoomToolbox : public tgui::Tabs {
  private:
	int selectedTool = 0;
	std::vector<RoomToolboxItem> tools;
	tgui::CheckBox::Ptr brushToggle;

  public:
	typedef std::shared_ptr<RoomToolbox> Ptr;
	typedef std::shared_ptr<const RoomToolbox> ConstPtr;

	tgui::SignalTyped<RoomTool> onToolPressed = {"ToolPressed"};
	tgui::SignalTyped<bool> onBrushPressed = {"BrushPressed"};

	RoomToolbox(const char *typeName = "RoomToolbox", bool initRenderer = true);

	static RoomToolbox::Ptr create();

	static RoomToolbox::Ptr copy(RoomToolbox::ConstPtr widget);

	RoomToolboxRenderer *getSharedRenderer() override;

	const RoomToolboxRenderer *getSharedRenderer() const override;

	RoomToolboxRenderer *getRenderer() override;

	void rendererChanged(const tgui::String &property) override;

	tgui::Signal &getSignal(tgui::String signalName) override;

	bool isMouseOnWidget(tgui::Vector2f pos) const override;

	void draw(tgui::BackendRenderTarget &target,
			  tgui::RenderStates states) const override;

	void setSize(const tgui::Layout2d &size) override;

	bool leftMousePressed(tgui::Vector2f pos) override;

	void addTool(const std::string &resourcePath, RoomTool tool);

  protected:
	Widget::Ptr clone() const override;
};

#endif