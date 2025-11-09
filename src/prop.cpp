#include "prop.hpp"
#include "gamedata.hpp"
#include "game.hpp"
#include "interactable.hpp"
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
using json = nlohmann::json;

Prop::Prop() {}

Prop::Prop(std::string filePath)
{
    this->sourcePath = filePath;
    this->worldPos = Vector2 { 0, 0 };
    this->tilePos = Vector2 { 0, 0 };

    std::string jsonString = LoadFileText(filePath.c_str());

    json json = json::parse(jsonString);
    std::vector<int> atlasRectVec = json.at("atlas_rect");
    if (atlasRectVec.size() != 4) {
        throw std::runtime_error("Not enough items in atlas_rect.");
    }
    this->atlasRect = Rectangle {
        static_cast<float>(atlasRectVec[0]),
        static_cast<float>(atlasRectVec[1]),
        static_cast<float>(atlasRectVec[2]),
        static_cast<float>(atlasRectVec[3])
    };

    std::vector<int> collisionRectVec = json.at("collision_rect");
    if (collisionRectVec.size() != 4) {
        throw std::runtime_error("Not enough items in collision_rect.");
    }
    this->collisionRect = Rectangle {
        static_cast<float>(collisionRectVec[0]),
        static_cast<float>(collisionRectVec[1]),
        static_cast<float>(collisionRectVec[2]),
        static_cast<float>(collisionRectVec[3])
    };

    std::string imagePath = json.at("image");
    this->imagePath = imagePath;
    this->texture = LoadTexture(imagePath.c_str());

    this->hasInteractable = static_cast<bool>(json.at("has_interactable"));

    this->interactable = std::make_unique<IntBaseWrapper>();
    if (hasInteractable) {
        InteractableType intType = static_cast<InteractableType>(json.at("interactable_type"));
        this->interactable = make_inter_item(Vector2 { 0, 0 }, intType);
    }
}

Prop::Prop(Rectangle atlasRect, Vector2 worldPos)
{
    this->sourcePath = "";
    this->atlasRect = atlasRect;
    this->worldPos = worldPos;
    this->tilePos = Vector2 { static_cast<float>(worldPos.x / 16.0f), static_cast<float>(worldPos.y / 16.0f) };
    this->collisionRect = Rectangle { 0, 0, 16, 16 };
    this->hasInteractable = false;
    this->interactable = std::make_unique<IntBaseWrapper>();
}

Prop::Prop(PropBin bin)
{
    this->sourcePath = bin.name;
    this->worldPos = Vector2 { 0, 0 };
    this->tilePos = Vector2 { 0, 0 };
    this->atlasRect = Rectangle {
        static_cast<float>(bin.atlasRect.x), static_cast<float>(bin.atlasRect.y),
        static_cast<float>(bin.atlasRect.width), static_cast<float>(bin.atlasRect.height)
    };
    this->collisionRect = Rectangle {
        static_cast<float>(bin.collisionRect.x), static_cast<float>(bin.collisionRect.y),
        static_cast<float>(bin.collisionRect.width), static_cast<float>(bin.collisionRect.height)
    };

    ImageBin imgBin = Game::getBin().images.at(GetFileName(bin.imagePath.c_str()));
    Image img = LoadImageFromMemory(
        GetFileExtension(bin.imagePath.c_str()),
        imgBin.data.data(), imgBin.dataSize);
    setTexture(LoadTextureFromImage(img));
    UnloadImage(img);

    this->hasInteractable = bin.hasInteractable;
    this->interactable = std::make_unique<IntBaseWrapper>();
    if (hasInteractable) {
        this->interactable = make_inter_item(Vector2 { 0, 0 }, static_cast<InteractableType>(bin.intType));
    }
}

json Prop::dumpJson()
{
    std::vector<int> atlasRectVec = {
        static_cast<int>(atlasRect.x), static_cast<int>(atlasRect.y),
        static_cast<int>(atlasRect.width), static_cast<int>(atlasRect.height)
    };
    std::vector<int> collisionRectVec = {
        static_cast<int>(collisionRect.x), static_cast<int>(collisionRect.y),
        static_cast<int>(collisionRect.width), static_cast<int>(collisionRect.height)
    };

    int intNum = 0;
    if (hasInteractable) {
        intNum = static_cast<int>(interactable->type);
    }

    json j{
        {"atlas_rect", atlasRectVec},
        {"collision_rect", collisionRectVec},
        {"image", imagePath},
        {"has_interactable", static_cast<int>(hasInteractable)},
        {"interactable", intNum}
    };
    return j;
}

std::string Prop::getSourcePath()
{
    return sourcePath;
}

void Prop::setTexture(Texture2D texture)
{
    this->texture = texture;
}

Texture2D Prop::getTexture()
{
    return texture;
}

void Prop::setTextureFromPath(std::string imagePath)
{
    this->imagePath = imagePath;
    this->texture = LoadTexture(imagePath.c_str());
}

const char* Prop::getImagePath()
{
    return this->imagePath.c_str();
}

void Prop::setCollisionRect(Rectangle collisionRect)
{
    this->collisionRect = collisionRect;
}

void Prop::setWorldTilePos(Vector2 worldPos, int tileSize)
{
    this->worldPos = Vector2 {
        static_cast<float>(worldPos.x * tileSize),
        static_cast<float>(worldPos.y * tileSize)
    };
    this->tilePos = worldPos;
}

void Prop::setWorldPos(Vector2 worldPos)
{
    this->worldPos = worldPos;
}

Vector2 Prop::getWorldPos()
{
    return worldPos;
}

Vector2 Prop::getWorldTilePos()
{
    return tilePos;
}

void Prop::setAtlasRect(Rectangle atlasRect)
{
    this->atlasRect = atlasRect;
}

Rectangle Prop::getAtlasRect()
{
    return atlasRect;
}

Rectangle Prop::getCollisionRect()
{
    return collisionRect;
}

Rectangle Prop::getWorldCollisionRect()
{
    return Rectangle {
        worldPos.x + (collisionRect.x * RPGPP_DRAW_MULTIPLIER), worldPos.y + (collisionRect.y * RPGPP_DRAW_MULTIPLIER),
        collisionRect.width * RPGPP_DRAW_MULTIPLIER, collisionRect.height * RPGPP_DRAW_MULTIPLIER
    };
}

Vector2 Prop::getCollisionCenter()
{
    Rectangle rect = getWorldCollisionRect();
    return Vector2 {
        rect.x + (rect.width / 2), rect.y + (rect.height / 2)
    };
}

bool Prop::getHasInteractable()
{
    return hasInteractable;
}

IntBaseWrapper* Prop::getInteractable()
{
    return interactable.get();
}

void Prop::setInteractableType(InteractableType type)
{
    if (hasInteractable) {
        if (interactable->type == type) return;
    }
    this->interactable = make_inter_item(Vector2 { 0, 0 }, type);
    this->hasInteractable = true;
}

void Prop::draw()
{
    Rectangle dest = Rectangle {
        worldPos.x, worldPos.y,
        atlasRect.width * RPGPP_DRAW_MULTIPLIER, atlasRect.height * RPGPP_DRAW_MULTIPLIER
    };
    DrawTexturePro(texture, atlasRect, dest, Vector2 { 0, 0 }, 0.0f, WHITE);

    DrawRectangleRec(getWorldCollisionRect(), Fade(RED, 0.5f));
}
