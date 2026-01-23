#ifndef _RPGPP_WORLDVIEW_H
#define _RPGPP_WORLDVIEW_H

#include "TGUI/Backend/Renderer/Raylib/CanvasRaylib.hpp"
#include "TGUI/Widget.hpp"
#include "raylib.h"
#include "updatable.hpp"
#include <memory>

class WorldView : public tgui::CanvasRaylib, public IUpdatable {
  protected:
	Camera2D camera;
	RenderTexture texture;
	bool mouseMiddleButton;

  public:
	typedef std::shared_ptr<WorldView> Ptr;
	typedef std::shared_ptr<const WorldView> ConstPtr;

	WorldView(const char *typeName = "WorldView", bool initRenderer = true);

	static WorldView::Ptr create();
	static WorldView::Ptr copy(const WorldView::ConstPtr &widget);

	void setSize(const tgui::Layout2d &size) override;
	bool isMouseOnWidget(tgui::Vector2f pos) const override;
	void draw(tgui::BackendRenderTarget &target,
			  tgui::RenderStates states) const override;

	void mouseMoved(tgui::Vector2f pos) override;
	bool scrolled(float delta, tgui::Vector2f pos, bool touch) override;

	virtual void drawCanvas();

	void update() override;
	static std::shared_ptr<IUpdatable>
	asUpdatable(const std::shared_ptr<WorldView> &ptr);

  protected:
	tgui::Widget::Ptr clone() const override;
};

#endif