#include "editor.hpp"
#include "services/translationService.hpp"
#include <memory>

class TSConnection {
  public:
	TSConnection(TranslationService::ListenerID id) : id(id) {}
	~TSConnection() { Editor::instance->getTranslations().removeListener(id); }

  private:
	TranslationService::ListenerID id;
};

template <typename WidgetType>
void bindTranslation(std::shared_ptr<WidgetType> widget, const std::string &key,
					 void (WidgetType::*setter)(const tgui::String &)) {
	auto &ts = Editor::instance->getTranslations();
	std::weak_ptr<WidgetType> weakWidget = widget;

	// Initial set
	(widget.get()->*setter)(ts.getKey(key));

	auto id = ts.addListener(
		[weakWidget, key, setter](TranslationService::ListenerID id) {
			if (auto w = weakWidget.lock()) {
				auto &ts = Editor::instance->getTranslations();
				(w.get()->*setter)(ts.getKey(key));

			} else {
				// Editor::instance->getTranslations().removeListener(id);
			}
		});

	// TODO: Implement someway to detect when the widget is destroyed, and call
	//     Editor::instance->getTranslations().removeListener(id);
	// There's currently an issue on TGUI's repo to add support for callback
	// when a widget gets destroyed https://github.com/texus/TGUI/issues/326
}
