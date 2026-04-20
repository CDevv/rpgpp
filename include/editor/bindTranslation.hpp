#ifndef _RPGPP_BINDTRANSLATION_H
#define _RPGPP_BINDTRANSLATION_H

#include <functional>
#include <memory>

#include "editor.hpp"
#include "services/translationService.hpp"

class TSConnection {
public:
	TSConnection(TranslationService::ListenerID id) : id(id) {}
	~TSConnection() { Editor::instance->getTranslations().removeListener(id); }

private:
	TranslationService::ListenerID id;
};

// Binds a widget's text to a translation key, updating it whenever the translation changes.
template <typename WidgetType>
void bindTranslation(std::shared_ptr<WidgetType> widget, const std::string &key,
					 void (WidgetType::*setter)(const tgui::String &)) {
	auto &ts = Editor::instance->getTranslations();
	std::weak_ptr<WidgetType> weakWidget = widget;

	// Initial set
	(widget.get()->*setter)(ts.getKey(key));

	auto id = ts.addListener(
		[weakWidget, key, setter](TranslationService &ts, TranslationService::ListenerID id, bool checkingAlive) {
			if (auto w = weakWidget.lock()) {
				if (!checkingAlive) (w.get()->*setter)(ts.getKey(key));
				return true;
			} else {
				return false;
			}
		});
}

// Binds a custom callback to a widget, and calling it whenever the translation changes.
// The callback function provides references to the widget and the translation service.
template <typename WidgetType>
void bindTranslationWithCallback(std::shared_ptr<WidgetType> widget,
								 std::function<void(std::shared_ptr<WidgetType> widget, TranslationService &)> cb) {
	auto &ts = Editor::instance->getTranslations();
	std::weak_ptr<WidgetType> weakWidget = widget;

	cb(widget, ts);
	auto id =
		ts.addListener([weakWidget, cb](TranslationService &ts, TranslationService::ListenerID id, bool checkingAlive) {
			if (auto w = weakWidget.lock()) {
				if (!checkingAlive) cb(w, ts);
				return true;
			} else {
				return false;
			}
		});
}

#endif
