#include "editor.hpp"
#include "services/translationService.hpp"
#include <functional>
#include <memory>

class TSConnection {
  public:
	TSConnection(TranslationService::ListenerID id) : id(id) {}
	~TSConnection() { Editor::instance->getTranslations().removeListener(id); }

  private:
	TranslationService::ListenerID id;
};

// TODO: Can do the removing of dead listeners by looping and purging dead listeners every addListener call, before all the other logic
// Or do removing of dead listeners when change language. looping and purging before the other loop that call the callback

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

	// TODO: Implement someway to detect when the widget is destroyed, and call
	//     Editor::instance->getTranslations().removeListener(id);
	// There's currently an issue on TGUI's repo to add support for callback
	// when a widget gets destroyed https://github.com/texus/TGUI/issues/326
}

template <typename WidgetType>
void bindCustomTranslation(std::shared_ptr<WidgetType> widget, std::function<void(std::shared_ptr<WidgetType> widget, TranslationService&)> cb) {
	auto &ts = Editor::instance->getTranslations();
	std::weak_ptr<WidgetType> weakWidget = widget;

	cb(widget, ts);
	auto id = ts.addListener(
		[weakWidget, cb](TranslationService &ts, TranslationService::ListenerID id, bool checkingAlive) {
			if (auto w = weakWidget.lock()) {
				if (!checkingAlive) cb(w, ts);
				return true;
			} else {
				return false;
			}
		}
	);
}
