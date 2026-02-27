#include "fileViews/propFileView.hpp"
#include "editor.hpp"
#include "prop.hpp"
#include "variant.hpp"
#include "views/propView.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"

PropFileView::PropFileView() {
	TranslationService &ts = Editor::instance->getTranslations();

	propView = PropView::create();
	propView->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), "100%"});
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(propView));
	widgetContainer.push_back(propView);


	auto propBox = PropertiesBox::create();
	propBox->setSize({RIGHT_PANEL_W, "100%"});
	propBox->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});

	hasInteractableField = BoolField::create();
	hasInteractableField->label->setText(ts.getKey("screen.project.propview.has_interactable"));
	hasInteractableField->value->onChange([this](bool value) {
		propView->getProp()->setHasInteractable(value);
	});
	propBox->addBooleanField(hasInteractableField);

	propImageField = FileField::create();
	propImageField->label->setText(ts.getKey("screen.project.propview.image"));
	propImageField->pathFilters = {{"Image", {"*.png"}}}; // TODO: Add more image types
	propImageField->callback = [this](const tgui::String &path) {
		propView->getProp()->setTextureFromPath(path.toStdString());
	};;
	propBox->addFileField(propImageField);

	// atlasWidthField = IntField::create();
	// atlasWidthField->label->setText(ts.getKey("screen.project.propview.atlas.width"));
	// atlasWidthField->value->onValueChange([this](int value) {

	// });
	// propBox->addIntField(atlasWidthField);

	widgetContainer.push_back(propBox);
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
	hasInteractableField->value->setChecked(prop->getHasInteractable());
	propImageField->value->setText(prop->getImagePath());
	addWidgets(layout);
}
