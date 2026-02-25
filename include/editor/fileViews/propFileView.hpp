#ifndef _RPGPP_FILEVIEWS_TILESETFILEVIEW_H
#define _RPGPP_FILEVIEWS_TILESETFILEVIEW_H

#include "fileView.hpp"
#include "variant.hpp"

class PropFileView : public FileView {
public:
	PropFileView(const std::string& path);
	virtual ~PropFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;

private:
	void handleModePress(tgui::Vector2f pos);

};

#endif
