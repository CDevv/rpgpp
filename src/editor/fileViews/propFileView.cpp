#include "fileViews/propFileView.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "editor.hpp"
#include "prop.hpp"
#include "variant.hpp"
#include "views/propView.hpp"

PropFileView::PropFileView() {
	propView = PropView::create();
	propView->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), "100%"});
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(propView));
	widgetContainer.push_back(propView);
}

void PropFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;
	if (variant == nullptr) {
		return;
	}

	auto ptr = dynamic_cast<Variant<Prop> *>(variant);
	auto prop = ptr->get();
	if (prop == nullptr)
		return;

	propView->setProp(prop);
	addWidgets(layout);
}
