#include "fileViews/propFileView.hpp"
#include "editor.hpp"
#include "prop.hpp"
#include "raylib.h"
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

	auto propBox = PropertiesBox::create();
	propBox->setSize({RIGHT_PANEL_W, "100%"});
	propBox->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});

	hasInteractableField = BoolField::create();
	hasInteractableField->label->setText(
		ts.getKey("screen.project.propview.has_interactable"));
	hasInteractableField->value->onChange(
		[this](bool value) { propView->getProp()->setHasInteractable(value); });
	propBox->addBooleanField(hasInteractableField);

	propImageField = FileField::create();
	propImageField->label->setText(ts.getKey("screen.project.propview.image"));
	propImageField->pathFilters = {
		{"Image", {"*.png"}}}; // TODO: Add more image types
	propImageField->callback = [this](const tgui::String &path) {
		propView->getProp()->setTextureFromPath(path.toStdString());
	};
	;
	propBox->addFileField(propImageField);

	atlasRectField = RectangleField::create();
	atlasRectField->label->setText(ts.getKey("screen.project.propview.atlas"));
	atlasRectField->onChange(
		[this](Rectangle r) { propView->updateAtlasRect(r); });
	propBox->addRectangleField(atlasRectField);

	collisionsField = RectangleField::create();
	collisionsField->label->setText(
		ts.getKey("screen.project.propview.collision"));
	collisionsField->onChange(
		[this](Rectangle r) { propView->updateCollisionRect(r); });
	propBox->addRectangleField(collisionsField);

	propView->onUpdatedAtlasRect(
		[this](Rectangle r) { atlasRectField->setValue(r); });
	propView->onUpdatedCollisionRect(
		[this](Rectangle r) { collisionsField->setValue(r); });
	widgetContainer.push_back(propBox);
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
	hasInteractableField->value->setChecked(prop->getHasInteractable());
	propImageField->value->setText(GetFileName(prop->getImagePath()));
	atlasRectField->setValue(prop->getAtlasRect());
	collisionsField->setValue(prop->getCollisionRect());
	addWidgets(layout);
}
