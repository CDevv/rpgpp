#include "room.hpp"
#include "collisionsContainer.hpp"
#include "interactable.hpp"
#include "interactablesContainer.hpp"
#include "tilemap.hpp"
#include <memory>
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <vector>
using json = nlohmann::json;

Room::Room()
{
    this->lock = false;

    Camera2D camera;
    camera.offset = Vector2 { 0, 0 };
    camera.target = Vector2 { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    this->camera = camera;

    this->worldTileSize = 48;
    this->musicSource = "";
    this->interactables = std::make_unique<InteractablesContainer>();
    this->collisions = std::make_unique<CollisionsContainer>();
    this->props = std::make_unique<std::vector<Prop>>();
    this->tileMap = std::unique_ptr<TileMap>{};
    this->actors = std::unique_ptr<std::vector<Actor>>{};
    this->player = std::unique_ptr<Player>{};

    Prop p(Rectangle { 0, 0, 16, 16 }, Vector2 { 0, 48 });
    p.setTexture(Game::getResources().getTexture("prop.png"));
    p.setCollisionRect(Rectangle { 0, 8, 16, 8 });
    addProp(p);
}

Room::Room(std::string fileName)
{
    this->lock = false;

    Camera2D camera;
    camera.offset = Vector2 { 0, 0 };
    camera.target = Vector2 { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    this->camera = camera;

    this->musicSource = "";
    this->worldTileSize = 48;

    char* jsonString = LoadFileText(fileName.c_str());
    json roomJson = json::parse(jsonString);

    std::vector<int> startPosVec = roomJson.at("start_pos");
    startTile = Vector2 { static_cast<float>(startPosVec[0]), static_cast<float>(startPosVec[1]) };

    this->interactables = std::make_unique<InteractablesContainer>();
    this->collisions = std::make_unique<CollisionsContainer>();
    this->actors = std::make_unique<std::vector<Actor>>();

    this->tileMap = std::make_unique<TileMap>(fileName);
    std::unique_ptr<Actor> actor = std::make_unique<Actor>("actors/playerActor.ractor");
    actor->setTilePosition(Vector2 {startTile.x, startTile.y}, tileMap->getTileSet()->getTileSize());
    std::unique_ptr<Player> player = std::make_unique<Player>(std::move(actor), *this);

    this->addPlayer(std::move(player));

    std::vector<std::vector<int>> collisionsVec = roomJson.at("collision");
    for (auto v : collisionsVec) {
        int x = v[0];
        int y = v[1];

        collisions->addCollisionTile(x, y);
    }

    interactables->addJsonData(roomJson);

    musicSource = roomJson.at("music_source");

    UnloadFileText(jsonString);
}

Room::Room(std::unique_ptr<TileMap> tileMap)
: Room()
{
    this->tileMap = std::move(tileMap);
}

Room::Room(RoomBin bin)
: Room()
{
    this->worldTileSize = 48;

    this->interactables = std::make_unique<InteractablesContainer>();
    this->collisions = std::make_unique<CollisionsContainer>();
    this->actors = std::make_unique<std::vector<Actor>>();

    this->tileMap = std::make_unique<TileMap>(bin);

    std::unique_ptr<Actor> actor = std::make_unique<Actor>(Game::getBin().actors.at(0));
    actor->setTilePosition(Vector2 {
        static_cast<float>(bin.startPoint.x), static_cast<float>(bin.startPoint.y)
    }, tileMap->getTileSet()->getTileSize());
    std::unique_ptr<Player> player = std::make_unique<Player>(std::move(actor), *this);

    this->addPlayer(std::move(player));

    interactables->addBinVector(bin.interactables);

    musicSource = bin.musicSource;
    if (!bin.musicSource.empty()) {
        Game::getSounds().loadMusic(bin.musicSource);
        Game::getSounds().playMusic();
    }

    for (auto collisionBin : bin.collisions) {
        collisions->addCollisionTile(collisionBin.x, collisionBin.y);
    }
}

json Room::dumpJson()
{
    json roomJson = this->tileMap->dumpJson();

    //Vector for collisions
    auto collisionsVector = std::vector<std::vector<int>>();
    for (Vector2 collisionPos : collisions->getVector()) {
        std::vector<int> collision;
        collision.push_back(collisionPos.x);
        collision.push_back(collisionPos.y);

        collisionsVector.push_back(collision);
    }
    roomJson.push_back({"collision", collisionsVector});

    //Vector for interactables
    auto interactablesProps = std::map<std::string, std::vector<std::string>>();

    auto interactablesVector = std::vector<std::vector<int>>();
    for (auto&& interactable : interactables->getList()) {
        std::vector<int> interactableVector;
        interactableVector.push_back(interactable->pos.x);
        interactableVector.push_back(interactable->pos.y);
        interactableVector.push_back(static_cast<int>(interactable->type));
        interactableVector.push_back(static_cast<int>(interactable->onTouch));

        std::vector<std::string> propsVec;
        if (interactable->type == INT_TWO) {
            IntBase<DiagInt>* dialogueInter = static_cast<IntBase<DiagInt>*>(interactable);
            std::string key = TextFormat("%i;%i",
                static_cast<int>(interactable->pos.x),
                static_cast<int>(interactable->pos.y));
            propsVec.push_back(dialogueInter->get().dialogueSource);
            interactablesProps[key] = propsVec;
        }
        if (interactable->type == INT_WARPER) {
            IntBase<WarperInt>* dialogueInter = static_cast<IntBase<WarperInt>*>(interactable);
            std::string key = TextFormat("%i;%i",
                static_cast<int>(interactable->pos.x),
                static_cast<int>(interactable->pos.y));
            propsVec.push_back(dialogueInter->get().targetRoom);
            interactablesProps[key] = propsVec;
        }

        interactablesVector.push_back(interactableVector);
    }
    roomJson.push_back({"interactables", interactablesVector});
    roomJson.push_back({"interactable_props", interactablesProps});
    roomJson.push_back({"music_source", musicSource});
    roomJson.push_back({"start_pos", {
        static_cast<int>(startTile.x), static_cast<int>(startTile.y)
    }});

    return roomJson;
}

void Room::unload()
{
    tileMap->unload();

    for (auto&& actor : *actors) {
        actor.unload();
    }

    player->unload();
}

void Room::update()
{
    for (auto&& actor : *actors) {
        actor.update();
    }
    player->update();
    if (!lock) updateCamera();
}

void Room::updateCamera()
{
    Vector2 playerPos = player->getPosition();
    Vector2 cameraOffset = Vector2 { 0, 0 };
    Vector2 cameraTarget = Vector2 { 0, 0 };

    if (playerPos.x >= 320.0f) {
        cameraOffset.x = 320.0f;
        cameraTarget.x = playerPos.x;
    }
    if (playerPos.y >= 240.0f) {
        cameraOffset.y = 240.0f;
        cameraTarget.y = playerPos.y;
    }
    camera.target = cameraTarget;
    camera.offset = cameraOffset;
}

void Room::draw()
{
    BeginMode2D(camera);

    this->tileMap->draw();
    for (auto i : interactables->getList()) {
        Rectangle rect = Rectangle {
            i->pos.x * getWorldTileSize(), i->pos.y * getWorldTileSize(),
            static_cast<float>(getWorldTileSize()), static_cast<float>(getWorldTileSize())
        };
        DrawRectangleRec(rect, Fade(YELLOW, 0.5f));
    }
    for (auto c : collisions->getVector()) {
        Rectangle rect = Rectangle {
            c.x * worldTileSize, c.y * worldTileSize,
            static_cast<float>(worldTileSize), static_cast<float>(worldTileSize)
        };
        DrawRectangleRec(rect, Fade(RED, 0.5f));
    }

    for (auto p : *props) {
        if (p.getCollisionCenter().y <= player->getCollisionPos().y) {
            p.draw();
        }
    }

    for (auto&& actor : *actors) {
        actor.draw();
    }
    player->draw();

    for (auto p : *props) {
        if (p.getCollisionCenter().y > player->getCollisionPos().y) {
            p.draw();
        }
    }

    EndMode2D();
}

int Room::getWorldTileSize()
{
    return worldTileSize;
}

void Room::setLock(bool val)
{
    this->lock = val;
}

void Room::addActor(Actor actor) {
    this->actors->push_back(std::move(actor));
}

void Room::addPlayer(std::unique_ptr<Player> player)
{
    this->player = std::move(player);
}

Player& Room::getPlayer()
{
    return *player;
}

TileMap *Room::getTileMap()
{
    return this->tileMap.get();
}

std::vector<Vector2> Room::getCollisionTiles()
{
    return this->collisions->getVector();
}

std::string Room::getMusicSource()
{
    return musicSource;
}

void Room::setMusicSource(std::string musicSource)
{
    this->musicSource = musicSource;
}

Vector2 Room::getStartTile()
{
    return startTile;
}

void Room::setStartTile(Vector2 startTile)
{
    this->startTile = startTile;
}

CollisionsContainer& Room::getCollisions()
{
    return *this->collisions;
}

InteractablesContainer& Room::getInteractables()
{
    return *this->interactables;
}

std::vector<Prop>& Room::getProps()
{
    return *props;
}

void Room::addProp(Prop prop)
{
    props->push_back(prop);
}

void Room::removeProp(Vector2 worldPos)
{
    for (std::vector<Prop>::iterator it = props->begin(); it < props->end(); ++it) {
        if (it->getWorldPos().x == worldPos.x && it->getWorldPos().y == worldPos.y) {
            props->erase(it);
        }
    }
}
