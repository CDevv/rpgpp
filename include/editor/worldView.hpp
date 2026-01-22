#ifndef _RPGPP_WORLDVIEW_H
#define _RPGPP_WORLDVIEW_H

#include "TGUI/Backend/Renderer/Raylib/CanvasRaylib.hpp"
#include "TGUI/Widget.hpp"
#include "raylib.h"
#include "updatable.hpp"
#include <memory>

class WorldView : public tgui::CanvasRaylib, public IUpdatable {
  private:
	tgui::CanvasRaylib::Ptr canvas;
	RenderTexture texture;

  public:
	typedef std::shared_ptr<WorldView> Ptr;
	typedef std::shared_ptr<const WorldView> ConstPtr;

	WorldView(const char *typeName = "WorldView", bool initRenderer = true);
	//~WorldView();

	static WorldView::Ptr create();
	static WorldView::Ptr copy(WorldView::ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;
	bool isMouseOnWidget(tgui::Vector2f pos) const override;
	void draw(tgui::BackendRenderTarget &target,
			  tgui::RenderStates states) const override;

	void update() override;
	static std::shared_ptr<IUpdatable>
	asUpdatable(std::shared_ptr<WorldView> ptr);

  protected:
	tgui::Widget::Ptr clone() const override;
};

#endif