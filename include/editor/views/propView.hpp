#ifndef RPGPP_PROPVIEWER
#define RPGPP_PROPVIEWER
#include "prop.hpp"
#include "views/worldView.hpp"
#include <memory>

class PropView : public WorldView {
public:
	typedef std::shared_ptr<PropView> Ptr;
	std::unique_ptr<Prop> p;

	static PropView::Ptr create(Prop *prop);
	static PropView::Ptr create();
	void setProp(std::unique_ptr<Prop> newProp);

	Prop *getProp() const;

	PropView();
	void drawCanvas() override;
	void drawOverlay() override;

	bool leftMousePressed(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;
};
#endif
