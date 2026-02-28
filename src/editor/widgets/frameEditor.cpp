#include "widgets/frameEditor.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/GrowHorizontalLayout.hpp"
#include "TGUI/Widgets/ToggleButton.hpp"
#include "actor.hpp"
#include "editor.hpp"
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

void FrameEditor::onFrameChange(){

}

void FrameEditor::init() {
	TranslationService &ts = Editor::instance->getTranslations();

	auto topBarLayout = tgui::GrowHorizontalLayout::create();
	topBarLayout->setSize({"100%", MAX_TOP_BAR_HEIGHT});
	topBarLayout->getRenderer()->setSpaceBetweenWidgets(5.0f);

	auto directionChooser = tgui::ComboBox::create();

	for (int i = 0; i < IDLE_ANIMATION_COUNT; i++)
		directionChooser->addItem(
			ts.getKey(TextFormat("screen.project.actorview.dir%d", i)));

	directionChooser->setSelectedItemByIndex(0);

	directionChooser->onItemSelect.connect([this](const size_t &index) {
		actorView->actor->changeAnimation(static_cast<Direction>(index * 2));
		actorView->actor->update();
	});

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

	this->add(topBarLayout);
}

FrameEditor::Ptr FrameEditor::create(ActorFileView *fileView) {
	return std::make_shared<FrameEditor>(fileView);
}

tgui::Widget::Ptr FrameEditor::clone() const {
	return std::make_shared<FrameEditor>(*this);
}
