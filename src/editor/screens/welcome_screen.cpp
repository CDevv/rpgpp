#include "welcome_screen.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Picture.hpp"
#include "TGUI/Widgets/VerticalLayout.hpp"
#include "editor.hpp"
#include "editorGuiService.hpp"
#include "translationService.hpp"

void screens::welcome_screen::add_elements(tgui::Gui *layout) {
  auto &ts = editor::current_editor->translation_service;
  // Create the logo in the center of the screen.
  auto welcome_picture = tgui::Picture::create("resources/logo-ups.png");
  welcome_picture->setOrigin({0.5, 0.5});
  welcome_picture->setPosition({"50%", "50%"});
  layout->add(welcome_picture);
  auto vertical_layout = tgui::GrowVerticalLayout::create();
  vertical_layout->setPosition(0, TOP_MARGIN);
  // TODO: Make the menu, actually get you started.
  // The "Get Started!" text.
  auto get_started_top_header = tgui::Label::create(
      ts->get_translation_by_key("screen.starting.get_started"));
  get_started_top_header->getRenderer()->setTextSize(30);
  vertical_layout->getRenderer()->setSpaceBetweenWidgets(5.0f);
  vertical_layout->add(get_started_top_header);
  vertical_layout->add(tgui::Label::create(
      ts->get_translation_by_key("screen.starting.description")));
  layout->add(vertical_layout);
}