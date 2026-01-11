#include "worldService.hpp"
#include <string_view>

WorldService::WorldService() {
  this->lock = false;
  this->room = std::unique_ptr<Room>{};
  this->deferRoomChange = false;
  this->deferredRoomId = "";
  this->frameCounter = 0;
  this->transitionActive = false;
  this->transitionColor = Color{0, 0, 0, 1};
  this->alpha = 0.0f;
  this->transitionSecondStage = false;
}

Room &WorldService::getRoom() const { return *this->room; }

void WorldService::setRoom(const std::string_view &filePath) {
  for (RoomBin bin : Game::getBin().rooms) {
    if (bin.name == filePath) {
      deferRoomChange = true;
      deferredRoomId = bin.name;
      break;
    }
  }
}

void WorldService::setRoomBin(RoomBin bin) {
  this->room = std::make_unique<Room>(bin);
}

void WorldService::doFadeTransition() {
  this->transitionActive = true;
  this->frameCounter = 0;
  this->transitionColor = Color{0, 0, 0, 1};
  this->alpha = 0.0f;
  this->transitionSecondStage = false;
}

Player &WorldService::getPlayer() const { return this->room->getPlayer(); }

void WorldService::update() {
  if (transitionActive) {
    frameCounter++;
    if (frameCounter >= 2) {
      frameCounter = 0;
      if (!transitionSecondStage) {
        printf("%f \n", alpha);
        if (alpha < 1.0f) {
          alpha += 0.02f;
          transitionColor = Fade(transitionColor, alpha);
        } else {
          transitionSecondStage = true;
          if (deferRoomChange) {
            for (RoomBin bin : Game::getBin().rooms) {
              if (bin.name == deferredRoomId) {
                setRoomBin(bin);
                break;
              }
            }
            deferRoomChange = false;
          }
        }
      } else {
        printf("%f \n", alpha);
        if (alpha > 0.0f) {
          alpha -= 0.02f;
          transitionColor = Fade(transitionColor, alpha);
        } else {
          transitionActive = false;
          transitionSecondStage = false;
        }
      }
    }
  }

  room->update();
}

void WorldService::draw() const {
  room->draw();

  if (transitionActive) {
    DrawRectangleRec(Rectangle{0, 0, static_cast<float>(GetScreenWidth()),
                               static_cast<float>(GetScreenHeight())},
                     transitionColor);
  }
}

void WorldService::unload() const {
  // room->unload();
}
