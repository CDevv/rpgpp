#include "player.hpp"
#include "interactable.hpp"
#include <raylib.h>
#include <raymath.h>

Player::Player(std::unique_ptr<Actor> actor, Room &room) : room(room) {
  this->lock = false;
  this->position = Vector2{0, 0};
  this->velocity = Vector2{0, 0};
  this->size = 48;

  Rectangle collisionRect = actor->getCollisionRect(Vector2{0, 0});
  this->interactableArea =
      Rectangle{collisionRect.x - 6, collisionRect.y - 6,
                collisionRect.width + 12, collisionRect.height + 12};

  this->actor = std::move(actor);
  this->idleDirection = RPGPP_DOWN_IDLE;
  this->currentDirection = RPGPP_DOWN;

  this->position = this->actor->getPosition();
}

void Player::unload() const { actor->unload(); }

void Player::update() {
  Rectangle collisionRect = actor->getCollisionRect(Vector2{0, 0});
  this->interactableArea =
      Rectangle{collisionRect.x - 6, collisionRect.y - 6,
                collisionRect.width + 12, collisionRect.height + 12};

  int change = 2;
  velocity = Vector2{0, 0};

  if (IsKeyDown(KEY_LEFT_SHIFT)) {
    change *= 2;
  }

  if (IsKeyDown(KEY_UP)) {
    currentDirection = RPGPP_UP;
    idleDirection = RPGPP_UP_IDLE;
    velocity.y -= change;
  } else if (IsKeyDown(KEY_DOWN)) {
    currentDirection = RPGPP_DOWN;
    idleDirection = RPGPP_DOWN_IDLE;
    velocity.y += change;
  } else if (IsKeyDown(KEY_LEFT)) {
    currentDirection = RPGPP_LEFT;
    idleDirection = RPGPP_LEFT_IDLE;
    velocity.x -= change;
  } else if (IsKeyDown(KEY_RIGHT)) {
    currentDirection = RPGPP_RIGHT;
    idleDirection = RPGPP_RIGHT_IDLE;
    velocity.x += change;
  } else if (IsKeyPressed(KEY_Z)) {
    this->handleInteraction();
  }

  if (!lock)
    this->handleCollision();

  if (lock)
    return;

  if (Vector2Equals(velocity, Vector2{0, 0})) {
    actor->changeAnimation(idleDirection);
  } else {
    actor->changeAnimation(currentDirection);
  }

  this->moveByVelocity(velocity);

  actor->update();
}

void Player::draw() const {
  actor->draw();

  // debug draw interactable area..
  Color interactableAreaDebugColor = ORANGE;
  interactableAreaDebugColor.a = (255 / 4);

  DrawRectangleRec(interactableArea, interactableAreaDebugColor);
}

void Player::handleCollision() {
  if (actor == nullptr)
    return;

  Rectangle playerRect = actor->getCollisionRect(velocity);

  TileMap *tileMap = room.getTileMap();
  int worldTileSize = tileMap->getWorldTileSize();

  // collision tiles
  std::vector<Vector2> collisionTiles = this->room.getCollisionTiles();
  for (Vector2 v : collisionTiles) {
    Rectangle tileRect = Rectangle{v.x * worldTileSize, v.y * worldTileSize,
                                   static_cast<float>(worldTileSize),
                                   static_cast<float>(worldTileSize)};

    if (CheckCollisionRecs(playerRect, tileRect)) {
      velocity = Vector2{0, 0};
      break;
    }
  }

  // props
  for (auto &&p : room.getProps()) {
    if (CheckCollisionRecs(playerRect, p.getWorldCollisionRect())) {
      velocity = Vector2{0, 0};
      break;
    }
  }

  // interactable tiles
  std::vector<IntBaseWrapper *> interactableTiles =
      this->room.getInteractables().getList();
  for (IntBaseWrapper *interactable : interactableTiles) {
    Rectangle tileRect =
        Rectangle{interactable->pos.x * room.getWorldTileSize(),
                  interactable->pos.y * room.getWorldTileSize(),
                  static_cast<float>(room.getWorldTileSize()),
                  static_cast<float>(room.getWorldTileSize())};

    if (CheckCollisionRecs(playerRect, tileRect)) {
      velocity = Vector2{0, 0};

      if (interactable->onTouch) {
        interactable->interact();
        if (interactable->type == INT_WARPER) {
          lock = true;
          room.setLock(true);
        }
      }

      break;
    }
  }
}

void Player::handleInteraction() {
  std::vector<IntBaseWrapper *> interactableTiles =
      this->room.getInteractables().getList();
  for (IntBaseWrapper *interactable : interactableTiles) {
    Rectangle tileRect =
        Rectangle{interactable->pos.x * room.getWorldTileSize(),
                  interactable->pos.y * room.getWorldTileSize(),
                  static_cast<float>(room.getWorldTileSize()),
                  static_cast<float>(room.getWorldTileSize())};

    if (CheckCollisionRecs(interactableArea, tileRect)) {
      if (interactable->type == INT_WARPER) {
        lock = true;
        room.setLock(true);
      }

      interactable->interact();

      break;
    }
  }

  for (auto &&p : room.getProps()) {
    if (p.getHasInteractable()) {
      if (CheckCollisionRecs(interactableArea, p.getWorldCollisionRect())) {
        p.getInteractable()->interact();
        break;
      }
    }
  }
}

void Player::setRoom(Room &room) const {
  // this->room = Room(room);
}

void Player::moveByVelocity(Vector2 velocity) {
  Vector2 resultVector = Vector2Add(position, velocity);
  position = resultVector;

  actor->moveByVelocity(velocity);
}

Vector2 Player::getPosition() const {
  if (actor == nullptr)
    return Vector2{0, 0};

  Rectangle actorRect = actor->getRect();
  return Vector2{actorRect.x + (actorRect.width / 2),
                 actorRect.y + (actorRect.height / 2)};
}

Vector2 Player::getCollisionPos() const {
  if (actor == nullptr)
    return Vector2{0, 0};

  return actor->getCollisionCenter();
}
