#include "fileViews/dialogueFileView.hpp"
#include "TGUI/Layout.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/FileDialog.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "TGUI/Widgets/TextArea.hpp"
#include "dialogue.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "services/fileSystemService.hpp"
#include "variant.hpp"
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include <memory>

DialogueFileView::DialogueFileView() {
	auto panel = tgui::Panel::create();
	panel->getRenderer()->setPadding({16, 16});

	vertLayout = tgui::GrowVerticalLayout::create();
	panel->add(vertLayout);
	widgetContainer.push_back(panel);
}

void DialogueFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<Dialogue> *>(variant);
		const auto dialogue = ptr->get();

		auto &data = dialogue->getData();

		int i = 0;
		for (auto line : dialogue->getData().lines) {
			auto panel = tgui::Panel::create({"100%", 200});
			panel->getRenderer()->setPadding({16, 16});

			tgui::Layout2d portraitLayout = {"100% - 40", "100% - 40"};

			auto portraitPic = tgui::Picture::create();
			portraitPic->setPosition(0, 40);
			portraitPic->setSize("height", "100% - 40");
			if (line.hasPortrait) {
				auto filePath = Editor::instance->getProject()->getResourcePath(
					EngineFileType::FILE_IMAGE, line.imageId);
				if (FileExists(filePath.c_str())) {
					tgui::Texture texture(filePath);
					portraitPic->getRenderer()->setTexture(texture);
				}
			}

			std::weak_ptr<tgui::Picture> portraitWeak = portraitPic;
			portraitPic->onClick([&data, i, portraitWeak] {
				if (data.lines.at(i).hasPortrait) {
					auto fileDialog = tgui::FileDialog::create();
					fileDialog->setFileTypeFilters(
						{{"Images", {"*.png", "*.jpg"}}});
					fileDialog->onFileSelect([&data, i, portraitWeak](
												 const tgui::String &path) {
						data.lines.at(i).imageId =
							GetFileName(path.toStdString().c_str());
						tgui::Texture texture(path);
						portraitWeak.lock()->getRenderer()->setTexture(texture);
					});

					Editor::instance->getGui().gui->add(fileDialog);
				}
			});
			panel->add(portraitPic);

			auto hasImageCheck = tgui::CheckBox::create("Has a portrait?");
			hasImageCheck->setSize(32, 32);
			hasImageCheck->setChecked(line.hasPortrait);
			hasImageCheck->onCheck([&data, i, &portraitPic](bool value) {
				data.lines.at(i).hasPortrait = value;
				if (!value) {
					data.lines.at(i).imageId = "";
				}
			});
			panel->add(hasImageCheck);

			auto charNameEdit = tgui::EditBox::create();
			charNameEdit->setSize({"100% - 210", 32});
			charNameEdit->setPosition(210, 0);
			charNameEdit->setText(line.characterName);
			charNameEdit->onTextChange(
				[&line, &data, i](const tgui::String &text) {
					line.characterName = text.toStdString();
					data.lines.at(i).characterName = text.toStdString();
				});
			panel->add(charNameEdit);

			auto diagTextEdit = tgui::TextArea::create();
			diagTextEdit->setPosition(210, 32 + 8);
			diagTextEdit->setSize({"100% - 210", "100% - 40"});
			diagTextEdit->setText(line.text);
			diagTextEdit->onTextChange([&data, i](const tgui::String &text) {
				data.lines.at(i).text = text.toStdString();
			});
			panel->add(diagTextEdit);

			vertLayout->add(panel);

			auto space = tgui::Panel::create({"100%", 20});
			space->getRenderer()->setOpacity(0.0f);
			vertLayout->add(space);

			i++;
		}

		addWidgets(layout);
	}
}