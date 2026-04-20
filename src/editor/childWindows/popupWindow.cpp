#include "childWindows/popupWindow.hpp"

#include <memory>

#include "TGUI/Animation.hpp"
#include "TGUI/Backend/raylib.hpp"
#include "TGUI/Duration.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "editor.hpp"
#include "services/editorGuiService.hpp"

constexpr const int ANIMATION_DURATION = 200;
constexpr const float TITLEBAR_HEIGHT = 30.0f;
constexpr tgui::ShowEffectType ANIMATION_TYPE = tgui::ShowEffectType::Scale;

PopupWindow::PopupWindow(const std::string &title) {
	this->currentWindow = tgui::ChildWindow::create(title);
	this->currentWindow->onClose.connect([this] { this->windowIsOpen = false; });
	this->currentWindow->onClosing.connect([this](bool *abort) {
		*abort = true;
		this->close();
	});
}

void PopupWindow::open() {
	if (this->windowIsOpen) return;

	std::unique_ptr<tgui::Gui> &gui = Editor::instance->getGui().gui;

	if (gui->getWidgetIndex(this->currentWindow) == -1) gui->add(this->currentWindow);

	// pop-up in the center of the screen.
	this->currentWindow->setPosition("(parent.innersize - size) / 2");
	;
	this->currentWindow->showWithEffect(ANIMATION_TYPE, tgui::Duration(ANIMATION_DURATION));
	this->windowIsOpen = true;
	this->currentWindow->moveToFront();
}

void PopupWindow::close() {
	if (!this->windowIsOpen) return;

	this->currentWindow->hideWithEffect(ANIMATION_TYPE, ANIMATION_DURATION);
	this->windowIsOpen = false;
}
