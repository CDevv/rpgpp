#include "player.hpp"
#include <raymath.h>

Player::Player(Actor *actor) {
    this->actor = actor;
    this->idleDirection = RPGPP_DOWN_IDLE;
    this->currentDirection = RPGPP_DOWN;
}

void Player::unload()
{
    actor->unload();
}

void Player::update()
{
    const int change = 2;
    Vector2 velocity = (Vector2){ 0, 0 };

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

    if (Vector2Equals(velocity, (Vector2){ 0, 0 })) {
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
}

void Player::moveByVelocity(Vector2 velocity)
{
    Vector2 resultVector = Vector2Add(position, velocity);
    position = resultVector;

    actor->moveByVelocity(velocity);
}


