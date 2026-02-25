#include "fileViews/propFileView.hpp"
#include "prop.hpp"
#include "variant.hpp"

void PropFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;
	if (variant == nullptr) {
		return;
	}

	const auto ptr = dynamic_cast<Variant<Prop> *>(variant);
	const auto prop = ptr->get();
	addWidgets(layout);
}
