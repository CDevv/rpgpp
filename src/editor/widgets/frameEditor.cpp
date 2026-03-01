#include "widgets/frameEditor.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/CheckBox.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/GrowHorizontalLayout.hpp"
#include "TGUI/Widgets/ToggleButton.hpp"
#include "editor.hpp"
#include "fileViews/actorFileView.hpp"
#include "raylib.h"
#include "services/translationService.hpp"
#include <cstddef>
#include <memory>

constexpr int IDLE_ANIMATION_COUNT = 4;
constexpr int MAX_TOP_BAR_HEIGHT = 30;

FrameEditor::FrameEditor(ActorFileView *fileView, const char *typeName,
						 bool initRenderer)
	: tgui::Panel(typeName, initRenderer), fileView(fileView),
	  actorView(fileView->getActorView()) {}

void FrameEditor::onFrameChange(int currentFrame) {}

void FrameEditor::init() {
	TranslationService &ts = Editor::instance->getTranslations();

	auto topBarLayout = tgui::GrowHorizontalLayout::create();
	topBarLayout->setSize({"100%", MAX_TOP_BAR_HEIGHT});
	topBarLayout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	this->directionChooser = tgui::ComboBox::create();

	for (int i = 0; i < IDLE_ANIMATION_COUNT; i++)
		directionChooser->addItem(
			ts.getKey(TextFormat("screen.project.actorview.dir%d", i)));

	directionChooser->setSelectedItemByIndex(0);

	directionChooser->onItemSelect.connect(
		[this](const size_t &index) { this->changeFrameState(index); });

	topBarLayout->add(directionChooser);

	playPauseButton =
		tgui::ToggleButton::create(ts.getKey("screen.project.actorview.play"));

	playPauseButton->onToggle.connect([&, this](const bool &checked) {
		playPauseButton->setText(
			checked ? ts.getKey("screen.project.actorview.pause")
					: ts.getKey("screen.project.actorview.play"));
		actorView->isPlaying = checked;
	});
	playPauseButton->setSize({100, "100%"});
	topBarLayout->add(playPauseButton);

	this->walkAnimation = tgui::CheckBox::create(
		ts.getKey("screen.project.actorview.is_non_idle"));
	walkAnimation->onChange.connect([this] { this->changeFrameState(); });
	walkAnimation->setSize(30, "100%");

	topBarLayout->add(this->walkAnimation);

	auto editingAtlasData = tgui::ToggleButton::create("Edit Animation Data");
	editingAtlasData->setSize({200, "100%"});
	editingAtlasData->onToggle.connect(
		[this](const bool isChecked) { actorView->editData = isChecked; });
	topBarLayout->add(editingAtlasData);

	this->add(topBarLayout);
}

FrameEditor::Ptr FrameEditor::create(ActorFileView *fileView) {
	return std::make_shared<FrameEditor>(fileView);
}

tgui::Widget::Ptr FrameEditor::clone() const {
	return std::make_shared<FrameEditor>(*this);
}

void FrameEditor::changeFrameState(int index) {
	this->actorView->actor->changeAnimation(static_cast<Direction>(
		(index * 2) + (this->walkAnimation->isChecked() ? 1 : 0)));
	this->actorView->actor->update();
}

void FrameEditor::changeFrameState() {
	this->actorView->actor->changeAnimation(
		static_cast<Direction>((directionChooser->getSelectedItemIndex() * 2) +
							   (this->walkAnimation->isChecked() ? 1 : 0)));
	this->actorView->actor->update();
}