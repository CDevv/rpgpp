#ifndef _RPGPP_PROPVIEWER_H
#define _RPGPP_PROPVIEWER_H
#include <list>
#include <memory>
#include <optional>

#include "TGUI/Signal.hpp"
#include "components/resizableCanvasBox.hpp"
#include "prop.hpp"
#include "raylib.h"
#include "views/worldView.hpp"

enum RectType { ATLAS_RECT, COLLISION_RECT };

class PropView : public WorldView {
public:
	PropView();
	typedef std::shared_ptr<PropView> Ptr;
	Prop *p{nullptr};

	static PropView::Ptr create(Prop *prop);
	static PropView::Ptr create();
	void setProp(Prop *newProp);

	Prop *getProp() const;

	void drawCanvas() override;
	void drawOverlay() override;

	bool leftMousePressed(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;
	void leftMouseReleased(tgui::Vector2f pos) override;

	std::optional<ResizableCanvasBox *> getAtlasRect();

	void updateAtlasRect(Rectangle r);
	void updateCollisionRect(Rectangle r);

	tgui::SignalTyped<Rectangle> onUpdatedAtlasRect = {"onUpdatedAtlasRect"};
	tgui::SignalTyped<Rectangle> onUpdatedCollisionRect = {"onUpdatedCollisionRect"};

private:
	std::list<ResizableCanvasBox> boxes;
	ResizableCanvasBox *focusedBox = nullptr;
};
#endif
