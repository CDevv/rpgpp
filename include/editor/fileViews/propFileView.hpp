#ifndef _RPGPP_FILEVIEWS_TILESETFILEVIEW_H
#define _RPGPP_FILEVIEWS_TILESETFILEVIEW_H

#include "fileView.hpp"
#include "variant.hpp"
#include "views/propView.hpp"

class PropFileView : public FileView {
public:
	PropFileView();
	virtual ~PropFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;

private:
	void handleModePress(tgui::Vector2f pos);
	PropView::Ptr propView;
};

#endif
