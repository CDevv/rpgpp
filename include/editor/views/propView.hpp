#ifndef _RPGPP_PROPVIEWER_H
#define _RPGPP_PROPVIEWER_H
#include "prop.hpp"
#include "views/worldView.hpp"
#include <memory>

class PropView : public WorldView {
public:
	PropView();
	typedef std::shared_ptr<PropView> Ptr;
	Prop* p{nullptr};

	static PropView::Ptr create(Prop *prop);
	static PropView::Ptr create();
	void setProp(Prop* newProp);

	Prop *getProp() const;

	void drawCanvas() override;
	void drawOverlay() override;

	bool leftMousePressed(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;
};
#endif
