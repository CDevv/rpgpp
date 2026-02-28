#include "fileViews/actorFileView.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "actor.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "raymath.h"
#include "services/translationService.hpp"
#include "views/actorView.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include <cstddef>



constexpr int TOP_SELECT_HEIGHT = 30;
constexpr int MAXIMUM_DIRECTION = 4;

ActorFileView::ActorFileView() {
	TranslationService& ts = Editor::instance->getTranslations();

	this->actorView = ActorView::create();

	Editor::instance->getGui().addUpdate(
		WorldView::asUpdatable(this->actorView));

	actorView->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), "100%"});
	actorView->setPosition(0, 0);
	widgetContainer.push_back(this->actorView);

	auto propBox = PropertiesBox::create();
	propBox->setSize({RIGHT_PANEL_W, "100%"});
	propBox->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), TOP_SELECT_HEIGHT});

	// Tilesets
	this->tileSetField = FileField::create();
	auto tileSetFileTr = ts.getKey("screen.project.roomview.tileset_file");
	tileSetField->label->setText(tileSetFileTr);
	tileSetField->pathFilters = {{tileSetFileTr, {"*.rtiles"}}};
	tileSetField->callback = [this](const tgui::String& text){
		actorView->actor->setTileSet(text.toStdString());
	};
	propBox->addFileField(tileSetField);

	// Is Walking Animation
	this->isNonIdle = BoolField::create();
	this->isNonIdle->label->setText(ts.getKey("screen.project.actorview.is_walk"));
	this->isNonIdle->value->onChange.connect([this]{ this->updateAnimationState(); });
	propBox->addBooleanField(this->isNonIdle);

	// Play and Pause Button
	this->playPauseButton = propBox->constructButton(
		ts.getKey("screen.project.actorview.play"),
	[&]{
		this->actorView->isPlaying = !actorView->isPlaying;
		this->playPauseButton->setText(actorView->isPlaying ? ts.getKey("screen.project.actorview.stop") : ts.getKey("screen.project.actorview.play"));
	});


	// The direction selector.
	this->directionSelect = tgui::ComboBox::create();
	directionSelect->setPosition(TextFormat("100%% - %d", RIGHT_PANEL_W), 0);
	directionSelect->setSize(RIGHT_PANEL_W, TOP_SELECT_HEIGHT);

	// Add the translation text possible in all 4 directions.
	for (int i = 0; i < MAXIMUM_DIRECTION; i++)
		directionSelect->addItem(ts.getKey(TextFormat("screen.project.actorview.dir%d",i)));
	directionSelect->setSelectedItemByIndex(0);
	directionSelect->onItemSelect.connect([this](const size_t& index) { this->updateAnimationState(); });

	widgetContainer.push_back(propBox);
	widgetContainer.push_back(directionSelect);
}

void ActorFileView::updateAnimationState(){
	if (this->directionSelect == nullptr) return;

	const auto& selectedIndex = this->directionSelect->getSelectedItemIndex() * 2;

	// the idle animations follow this order: 0, 2, 4, 6
	// to achieve the walking animation, add 1 to it.

	actorView->actor->changeAnimation(
		static_cast<Direction>(
			Clamp(selectedIndex + isNonIdle->value->isChecked(),
				0,
				RPGPP_MAX_DIRECTION
			)
		)
	);
}

void ActorFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<Actor> *>(variant);
		const auto ptrRaw = ptr->get();

		this->actorView->setActor(ptrRaw);
		this->tileSetField->setValue(GetFileName(ptrRaw->getTileSetSource().c_str()));

		addWidgets(layout);
	}
}
