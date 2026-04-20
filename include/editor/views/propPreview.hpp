#ifndef _RPGPP_ACTORPREVIEWVIEW_HPP
#define _RPGPP_ACTORPREVIEWVIEW_HPP

#include <memory>

#include "components/resizableCanvasBox.hpp"
#include "prop.hpp"
#include "views/worldView.hpp"

class PropFileView;

class PropPreview : public WorldView {
private:
	Prop *prop{nullptr};
	bool isOverBounds{false};
	ResizableCanvasBox *referenceToBox{nullptr};

public:
	PropPreview();

	typedef std::shared_ptr<PropPreview> Ptr;

	void setProp(Prop *prop);
	void setBox(ResizableCanvasBox *box);

	void drawCanvas() override;
	void drawOverlay() override;

	static PropPreview::Ptr create();
	static PropPreview::Ptr create(Prop *prop);
};

#endif