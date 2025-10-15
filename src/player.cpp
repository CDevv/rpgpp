#include "player.hpp"
#include <raymath.h>

Player::Player(std::unique_ptr<Actor> actor, Room& room)
: room(room)
{
    this->position = Vector2 { 0, 0 };
    this->velocity = Vector2 { 0, 0 };
    this->size = 48;

    Rectangle collisionRect = actor->getCollisionRect(Vector2 { 0, 0 });
    this->interactableArea = Rectangle {
        collisionRect.x - 6, collisionRect.y - 6,
        collisionRect.width + 12, collisionRect.height + 12
    };

    this->actor = std::move(actor);
    this->idleDirection = RPGPP_DOWN_IDLE;
    this->currentDirection = RPGPP_DOWN;

    this->position = this->actor->getPosition();
}

void Player::unload()
{
    actor->unload();
}

void Player::update()
{
    Rectangle collisionRect = actor->getCollisionRect(Vector2 { 0, 0 });
    this->interactableArea = Rectangle {
        collisionRect.x - 6, collisionRect.y - 6,
        collisionRect.width + 12, collisionRect.height + 12
    };

    int change = 2;
    velocity = Vector2 { 0, 0 };

    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        change *= 2;
    }

    if (IsKeyDown(KEY_UP)) {
        currentDirection = RPGPP_UP;
        idleDirection = RPGPP_UP_IDLE;
        velocity.y -= change;
    }
    else if (IsKeyDown(KEY_DOWN)) {
        currentDirection = RPGPP_DOWN;
        idleDirection = RPGPP_DOWN_IDLE;
        velocity.y += change;
    }
    else if (IsKeyDown(KEY_LEFT)) {
        currentDirection = RPGPP_LEFT;
        idleDirection = RPGPP_LEFT_IDLE;
        velocity.x -= change;
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        currentDirection = RPGPP_RIGHT;
        idleDirection = RPGPP_RIGHT_IDLE;
        velocity.x += change;
    }
    else if (IsKeyPressed(KEY_Z)) {
        this->handleInteraction();
    }

    this->handleCollision();

    if (Vector2Equals(velocity, Vector2 { 0, 0 })) {
        actor->changeAnimation(idleDirection);
    } else {
        actor->changeAnimation(currentDirection);
    }

    this->moveByVelocity(velocity);

    actor->update();
}

void Player::draw()
{
    actor->draw();

    //debug draw interactable area..
    Color interactableAreaDebugColor = ORANGE;
    interactableAreaDebugColor.a = (255 / 4);

    DrawRectangleRec(interactableArea, interactableAreaDebugColor);
}

void Player::handleCollision()
{
    Rectangle playerRect = actor->getCollisionRect(velocity);

    TileMap *tileMap = room.getTileMap();
    int worldTileSize = tileMap->getWorldTileSize();

    //collision tiles
    std::vector<Vector2> collisionTiles = this->room.getCollisionTiles();
    for (Vector2 v : collisionTiles) {
        Rectangle tileRect = Rectangle {
            v.x * worldTileSize, v.y * worldTileSize,
            (float)worldTileSize, (float)worldTileSize
        };

        if (CheckCollisionRecs(playerRect, tileRect)) {
            velocity = Vector2 { 0, 0 };
            break;
        }
    }

    //interactable tiles
    std::vector<IntBaseWrapper*> interactableTiles = this->room.getInteractables().getList();
    for (IntBaseWrapper* interactable : interactableTiles) {
        Rectangle tileRect = Rectangle {
            interactable->pos.x * room.getWorldTileSize(), interactable->pos.y * room.getWorldTileSize(),
            static_cast<float>(room.getWorldTileSize()), static_cast<float>(room.getWorldTileSize())
        };

        if (CheckCollisionRecs(playerRect, tileRect)) {
            velocity = Vector2 { 0, 0 };

            if (interactable->onTouch) {
                interactable->interact();
            }

            break;
        }
    }
}

void Player::handleInteraction()
{
    std::vector<IntBaseWrapper*> interactableTiles = this->room.getInteractables().getList();
    for (IntBaseWrapper* interactable : interactableTiles) {
        Rectangle tileRect = Rectangle {
            interactable->pos.x * room.getWorldTileSize(), interactable->pos.y * room.getWorldTileSize(),
            static_cast<float>(room.getWorldTileSize()), static_cast<float>(room.getWorldTileSize())
        };

        if (CheckCollisionRecs(interactableArea, tileRect)) {
            interactable->interact();
            break;
        }
    }
}

void Player::setRoom(Room& room)
{
    //this->room = Room(room);
}

void Player::moveByVelocity(Vector2 velocity)
{
    Vector2 resultVector = Vector2Add(position, velocity);
    position = resultVector;

    actor->moveByVelocity(velocity);
}
