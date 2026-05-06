#include "fileViews/interfaceViewFileView.hpp"

#include "TGUI/Widgets/TreeView.hpp"
#include "interfaceView.hpp"
#include "raylib.h"
#include "uiElement.hpp"

InterfaceViewFileView::InterfaceViewFileView() {
	treeView = tgui::TreeView::create();
	treeView->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});
	treeView->setSize({RIGHT_PANEL_W, "100%"});

	widgetContainer.push_back(treeView);
}

void InterfaceViewFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<InterfaceView> *>(variant);
		const auto interface = ptr->get();

		treeView->removeAllItems();
		for (auto &item : interface->getElements()) {
			treeView->addItem({item.second->getName()});
		}

		addWidgets(layout);
	}
}