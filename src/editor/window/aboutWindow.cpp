#include "windows/aboutWindow.hpp"
#include "TGUI/Cursor.hpp"
#include "editor.hpp"
#include "game.hpp"
#include "windowPopup.hpp"
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

AboutWindow::AboutWindow() : rect(Rectangle{}) {}

AboutWindow::AboutWindow(Rectangle rect) : WindowPopup("About RPG++", rect) {
  this->rect = rect;

  auto aboutWindow = tgui::ChildWindow::create("About..");
  aboutWindow->setSize({350, 300});

  auto aboutLayout = tgui::GrowVerticalLayout::create();
  aboutLayout->setSize({"90%", "90%"});
  aboutLayout->setPosition({"5%", "5%"});
  aboutLayout->getRenderer()->setSpaceBetweenWidgets(10.0f);

  auto image = tgui::Picture::create("resources/logo-ups.png");
  aboutLayout->add(image);
  // VERSION
  auto versionLabel = tgui::Label::create(TextFormat("RPG++ %s", RPGPP_VER));
  aboutLayout->add(versionLabel);
  // RAYLIB
  auto raylibLabel =
      tgui::Label::create(TextFormat("raylib %s", RAYLIB_VERSION));
  aboutLayout->add(raylibLabel);
  // TGUI
  auto tguiLabel = tgui::Label::create(TextFormat("TGUI %s", "1.1"));
  aboutLayout->add(tguiLabel);
  auto siteButton = tgui::Button::create("RPG++ Website");
  aboutLayout->add(siteButton);
  // Redirects to the site, had to fix due to Windows redirecting to "My
  // Documents"
  siteButton->setMouseCursor(tgui::Cursor::Type::Hand);
  siteButton->onClick.connect([] { OpenURL("https://rpgpp.dev"); });
  // Add the layout.
  aboutWindow->add(aboutLayout);
  this->childWindow = aboutWindow;
}

void AboutWindow::openWindow() { WindowPopup::openWindow(); }

void AboutWindow::closeWindow() { WindowPopup::closeWindow(); }

void AboutWindow::draw() {
  /*
  Texture2D logo = Editor::getResources().getTexture("logo");

  ImGui::SetNextWindowSize(ImVec2 { rect.width, -1 });
  if (ImGui::BeginPopupModal("About RPG++")) {
      rlImGuiImage(&logo);

      ImGui::TextLinkOpenURL("RPG++ Website", "https://rpgpp.dev");

      ImGui::Separator();

      ImGui::Text("RPG++ v%s", RPGPP_VER);
      ImGui::Text("raylib v%s", RAYLIB_VERSION);
      ImGui::Text("Dear ImGui v%s", IMGUI_VERSION);
      if (ImGui::Button("Close")) {
          closeWindow();
      }
      ImGui::EndPopup();
  }
  */
}
