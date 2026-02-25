#ifndef RPGPP_PROPVIEWER
#define RPGPP_PROPVIEWER
#include "prop.hpp"
#include "views/worldView.hpp"

class PropView : public WorldView {
public:
	typedef std::shared_ptr<PropView> Ptr;

	static PropView::Ptr create(Prop *prop);
	static PropView::Ptr create();
	void setProp(Prop *newProp);
	Prop *getProp();

	PropView(const std::string& path);
	virtual ~PropView();
	void drawCanvas() override;
	void drawOverlay() override;

	bool leftMousePressed(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;
};
#endif
