#include "fileViews/dialogueFileView.hpp"

#include <TGUI/Widgets/BitmapButton.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include <TGUI/Widgets/ScrollablePanel.hpp>
#include <algorithm>
#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "TGUI/Cursor.hpp"
#include "TGUI/Layout.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/FileDialog.hpp"
#include "TGUI/Widgets/GrowHorizontalLayout.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "TGUI/Widgets/TextArea.hpp"
#include "bindTranslation.hpp"
#include "childWindows/colorSelectWindow.hpp"
#include "dialogue.hpp"
#include "dialogueParser.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "services/fileSystemService.hpp"
#include "services/translationService.hpp"
#include "variant.hpp"
#include "widgets/dialogueEditor.hpp"

DialogueFileView::DialogueFileView() {
	TranslationService &ts = Editor::instance->getTranslations();
	noImageTexture = tgui::Texture(Editor::instance->getFs().getResourcePath("no-image.png"));

	deleteTexture = tgui::Texture(Editor::instance->getFs().getResourcePath("delete.png"));

	auto toolsPanel = tgui::Panel::create({"100%", 32});
	toolsPanel->getRenderer()->setPadding({4, 4});
	newLineButton = tgui::Button::create();

	bindTranslation<tgui::Button>(newLineButton, "screen.project.dialogueview.add_new_line", &tgui::Button::setText);
	newLineButton->setSize("20%", "100%");
	toolsPanel->add(newLineButton);

	widgetContainer.push_back(toolsPanel);

	mainPanel = tgui::ScrollablePanel::create({"100%", "100% - 32"});
	mainPanel->setPosition(0, 32);
	mainPanel->getRenderer()->setPadding({16, 16});

	mainPanel->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
	mainPanel->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);

	vertLayout = tgui::GrowVerticalLayout::create();
	vertLayout->getRenderer()->setSpaceBetweenWidgets(20.0f);
	mainPanel->add(vertLayout);
	widgetContainer.push_back(mainPanel);
}

tgui::Panel::Ptr DialogueFileView::makeLinePanel(DialogueBin &data, DialogueLine line, size_t i) {
	TranslationService &ts = Editor::instance->getTranslations();
	auto panel = tgui::Panel::create({"100%", DIALOGUE_PANEL_HEIGHT});
	panel->getRenderer()->setPadding({16, 16});

	tgui::Layout2d portraitLayout = {"100% - 40", "100% - 40"};

	auto portraitPic = tgui::Picture::create();
	portraitPic->setPosition(0, 40);
	portraitPic->setSize("height", "100% - 40");
	if (line.hasPortrait) {
		auto filePath = Editor::instance->getProject()->getResourcePath(EngineFileType::FILE_IMAGE, line.imageId);
		if (FileExists(filePath.c_str())) {
			tgui::Texture texture(filePath);
			portraitPic->getRenderer()->setTexture(texture);
		}
	} else {
		portraitPic->getRenderer()->setTexture(noImageTexture);
	}

	std::weak_ptr<tgui::Picture> portraitWeak = portraitPic;
	portraitPic->onClick([&data, i, portraitWeak] {
		if (data.lines.at(i).hasPortrait) {
			auto fileDialog = tgui::FileDialog::create();
			fileDialog->setFileTypeFilters({{"Images", {"*.png", "*.jpg"}}});
			fileDialog->onFileSelect([&data, i, portraitWeak](const tgui::String &path) {
				data.lines.at(i).imageId = GetFileName(path.toStdString().c_str());
				tgui::Texture texture(path);
				portraitWeak.lock()->getRenderer()->setTexture(texture);
			});

			Editor::instance->getGui().gui->add(fileDialog);
		}
	});
	panel->add(portraitPic);

	auto topControlsLayout = tgui::GrowHorizontalLayout::create();
	topControlsLayout->getRenderer()->setSpaceBetweenWidgets(5.0f);
	topControlsLayout->setSize({"100% - 210 - 40", 32});
	topControlsLayout->setPosition(210, 0);

	auto charNameEdit = tgui::EditBox::create();
	charNameEdit->setText(line.characterName);
	charNameEdit->onTextChange(
		[&data, i](const tgui::String &text) { data.lines.at(i).characterName = text.toStdString(); });
	topControlsLayout->add(charNameEdit);

	auto diagTextEdit = DialogueEditor::create();
	diagTextEdit->setPosition(210, 32 + 8);
	diagTextEdit->setMouseCursor(tgui::Cursor::Type::Text);
	diagTextEdit->setSize({"100% - 210 - 40", "100% - 40"});
	diagTextEdit->setText(line.text);
	diagTextEdit->onTextChange([&data, i](const tgui::String &text) { data.lines.at(i).text = text.toStdString(); });
	dialogueBoxes.push_back(diagTextEdit);

	auto selectColorButton = tgui::Button::create();
	bindTranslation<tgui::Button>(selectColorButton, "screen.project.dialogueview.select_a_color",
								  &tgui::Button::setText);

	std::weak_ptr<DialogueEditor> weakEditor = diagTextEdit;
	selectColorButton->onPress.connect([weakEditor] {
		if (auto capture = weakEditor.lock()) {
			if (capture->isSelectedTextEmpty()) {
				return;
			}

			auto selectColorWindow = reinterpret_cast<ColorSelectWindow *>(
				Editor::instance->getGui().getChildWindowSubService()->getWindow("select_a_color"));

			selectColorWindow->open(capture);
		}
	});

	topControlsLayout->add(selectColorButton);

	panel->add(topControlsLayout);
	panel->add(diagTextEdit);

	auto hasImageCheck = tgui::CheckBox::create();
	bindTranslation<tgui::CheckBox>(hasImageCheck, "screen.project.dialogueview.has_a_portrait",
									&tgui::CheckBox::setText);
	hasImageCheck->setSize(32, 32);
	hasImageCheck->setChecked(line.hasPortrait);

	std::weak_ptr<tgui::TextArea> weakDiagText = diagTextEdit;
	hasImageCheck->onCheck([this, &data, i, portraitPic, weakDiagText](bool value) {
		data.lines.at(i).hasPortrait = value;
		if (!value) {
			data.lines.at(i).imageId = "";
			if (!weakDiagText.expired()) {
				weakDiagText.lock()->setSize({"100%", "100% - 40"});
				weakDiagText.lock()->setPosition(0, 32 + 8);
			}

			portraitPic->getRenderer()->setTexture(noImageTexture);
		} else {
			if (!weakDiagText.expired()) {
				weakDiagText.lock()->setSize({"100% - 210 - 28", "100% - 40"});
				weakDiagText.lock()->setPosition(210, 32 + 8);
			}
		}
	});
	panel->add(hasImageCheck);

	auto deleteButton = tgui::BitmapButton::create();
	deleteButton->setSize({32, 32});
	deleteButton->setPosition("100% - 32", 0);
	deleteButton->setImage(deleteTexture);
	deleteButton->onClick([this, &data, i] {
		mainPanel->setContentSize({0, static_cast<float>((DIALOGUE_PANEL_HEIGHT + 8) * (data.lines.size()))});

		data.lines.erase(data.lines.begin() + i);
		vertLayout->remove(linePanels.at(i));
		linePanels.erase(linePanels.begin() + i);
	});
	panel->add(deleteButton);

	return panel;
}

void DialogueFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<Dialogue> *>(variant);
		const auto dialogue = ptr->get();

		auto &data = dialogue->getData();

		newLineButton->onClick([this, &dialogue, &data] {
			DialogueLine newLine;
			newLine.characterName = "Character";
			newLine.hasPortrait = false;
			newLine.imageId = "";
			newLine.text = "Enter text here!";

			data.lines.push_back(newLine);

			auto panel = makeLinePanel(data, newLine, data.lines.size() - 1);

			vertLayout->add(panel);
			linePanels.push_back(panel);

			mainPanel->setContentSize({0, static_cast<float>((DIALOGUE_PANEL_HEIGHT + 8) * (data.lines.size() + 1))});
		});

		mainPanel->setContentSize({0, static_cast<float>((DIALOGUE_PANEL_HEIGHT + 8) * (data.lines.size() + 1))});

		int i = 0;
		for (auto line : dialogue->getData().lines) {
			auto panel = makeLinePanel(data, line, i);

			i++;

			vertLayout->add(panel);
			linePanels.push_back(panel);
		}

		addWidgets(layout);
	}
}
