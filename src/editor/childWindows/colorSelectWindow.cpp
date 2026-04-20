#include "childWindows/colorSelectWindow.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "TGUI/Color.hpp"
#include "TGUI/Cursor.hpp"
#include "TGUI/Layout.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "bindTranslation.hpp"
#include "childWindows/popupWindow.hpp"
#include "dialogueParser.hpp"
#include "raylib.h"
#include "widgets/dialogueEditor.hpp"

constexpr int BLACK_WHITE_THRESHOLD = 80;

ColorSelectWindow::ColorSelectWindow() : PopupWindow("Select a Color") {
	this->editor = nullptr;

	bindTranslation<tgui::ChildWindow>(this->currentWindow, "screen.project.dialogueview.select_a_color",
									   &tgui::ChildWindow::setTitle);
	auto scrollablePanel = tgui::ScrollablePanel::create();
	auto verticalLayout = tgui::GrowVerticalLayout::create();
	verticalLayout->getRenderer()->setPadding(5.0f);

	std::vector<std::string> types = getColorTypes();
	std::map<std::string, Color> colors = getColors();
	for (int i = 0; i < types.size(); i++) {
		auto type = types[i];

		auto colorButton = tgui::Button::create();
		colorButton->setMouseCursor(tgui::Cursor::Type::Hand);

		bindTranslation<tgui::Button>(colorButton, TextFormat("screen.project.dialogueview.color.%s", type.c_str()),
									  &tgui::Button::setText);

		colorButton->setSize({"100%", 30});

		Color color = colors[types[i]];
		tgui::Color btnBackColor = {color.r, color.g, color.b, color.a};

		// Set the color of the background of the color picker.
		auto btnRenderer = colorButton->getRenderer();
		btnRenderer->setBackgroundColor(btnBackColor);
		btnRenderer->setBackgroundColorHover(btnBackColor);

		bool displayAsBlack = ((color.r + color.g + color.b) / 3.0f) > BLACK_WHITE_THRESHOLD;
		auto textColor = displayAsBlack ? tgui::Color::Black : tgui::Color::White;

		btnRenderer->setTextColor(textColor);
		btnRenderer->setTextColorHover(textColor);

		colorButton->onPress.connect([this, type] {
			this->editor->addXmlTag(type);
			this->close();
		});

		verticalLayout->add(colorButton);
	}

	scrollablePanel->add(verticalLayout);
	this->currentWindow->add(scrollablePanel);
}

void ColorSelectWindow::open(std::shared_ptr<DialogueEditor> editor) {
	PopupWindow::open();
	this->editor = editor;
}
