#include "childWindows/popupWindow.hpp"
#include "TGUI/Animation.hpp"
#include "TGUI/Duration.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "editor.hpp"

constexpr const int ANIMATION_DURATION = 200;
constexpr const float TITLEBAR_HEIGHT = 30.0f;
constexpr tgui::ShowEffectType ANIMATION_TYPE = tgui::ShowEffectType::Scale;

PopupWindow::PopupWindow(const std::string &title) {
	this->currentWindow = tgui::ChildWindow::create(title);
	this->currentWindow->onClose.connect(
		[this] { this->windowIsOpen = false; });
	this->currentWindow->onClosing.connect([this](bool *abort) {
		*abort = true;
		this->close();
	});
}

void PopupWindow::open() {
	if (this->windowIsOpen)
		return;

	Editor::instance->getGui().gui->add(this->currentWindow);
	// pop-up in the center of the screen.
	this->currentWindow->setOrigin({0.5, 0.5});
	this->currentWindow->setPosition({"50%", "50%"});
	this->currentWindow->showWithEffect(ANIMATION_TYPE,
										tgui::Duration(ANIMATION_DURATION));
	this->currentWindow->getRenderer()->setTitleBarHeight(TITLEBAR_HEIGHT);
	this->windowIsOpen = true;
}

void PopupWindow::close() {
	if (!this->windowIsOpen)
		return;

	this->currentWindow->hideWithEffect(ANIMATION_TYPE, ANIMATION_DURATION);
	this->windowIsOpen = false;
}