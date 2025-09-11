---
generator: doxide
---


# Actor

**class Actor**

The Actor class represents an Actor in the game's world.

:material-eye-outline: **See**
:    [Direction](Direction.md)


## Variables

| Name | Description |
| ---- | ----------- |
| [tileSet](#tileSet) | The used TileSet for this Actor's sprites. |
| [tileSetSource](#tileSetSource) | The path to the TileSet file. |
| [tile](#tile) | The currently shown tile from the TileSet. |
| [position](#position) | Current position in the world of this Actor. |
| [collisionRect](#collisionRect) | Rectangle, representing the collision of this Actor. |
| [animations](#animations) | An array of the animations. |
| [currentAnimation](#currentAnimation) | A Direction enum, showing the current animation that is being played. |

## Functions

| Name | Description |
| ---- | ----------- |
| [Actor](#Actor) | Constructor that takes a path to the .ractor file.  |
| [Actor](#Actor) | Constructor that takes a TileSet, the atlas position of the tile to use, and the path to the TileSet. |
| [Actor](#Actor) | Constructor that takes an ActorBin binary structure  |
| [dumpJson](#dumpJson) | Dump this Actor's data to a nlohmann::json object.  |
| [unload](#unload) | Unload routine. |
| [update](#update) | Update routine. |
| [draw](#draw) | Draw routine. |
| [getPosition](#getPosition) | Get this actor's position in the world. |
| [setPosition](#setPosition) | Set this Actor's position. |
| [getRect](#getRect) | Get the bounding box of this Actor. |
| [getTileSet](#getTileSet) | Get a reference to this Actor's TileSet. |
| [setTileSet](#setTileSet) | Set this Actor's TileSet using a path to the tileset file. |
| [moveByVelocity](#moveByVelocity) | Move the Actor using a velocity vector. |
| [getCollisionRect](#getCollisionRect) | Get the collision rectangle of this Actor if it was moved by the velocity vector  |
| [addAnimationFrame](#addAnimationFrame) | Add a frame in the chosen animation. |
| [removeAnimationFrame](#removeAnimationFrame) | Remove a frame in the chosen animation, specified by an index. |
| [setAnimationFrame](#setAnimationFrame) | Change a frame in the chosen animation to another atlas tile from the TileSet. |
| [addAnimationFrames](#addAnimationFrames) | Add multiple frames to the chosen animation. |
| [changeAnimation](#changeAnimation) | Change the Actor's current animation and play it. |
| [getTileSetSource](#getTileSetSource) | Get the path of the used TileSet. |
| [getAnimationsRaw](#getAnimationsRaw) | Get an array of this Actor's animations  |
| [getAnimationRaw](#getAnimationRaw) | Get a specific animation  |
| [getCollisionRect](#getCollisionRect) | Get the collision Rectangle of this Actor  |
| [setCollisionRect](#setCollisionRect) | Set the Actor's collision Rectangle  |

## Variable Details

### animations<a name="animations"></a>

!!! variable "std::array&lt;std::unique_ptr&lt;std::vector&lt;Vector2&gt;&gt;, 8&gt; animations"

    An array of the animations. Each std::vector<Vector2> represents the frames of an animation.
    

### collisionRect<a name="collisionRect"></a>

!!! variable "Rectangle collisionRect"

    Rectangle, representing the collision of this Actor.
    

### currentAnimation<a name="currentAnimation"></a>

!!! variable "Direction currentAnimation"

    A Direction enum, showing the current animation that is being played.
    

### position<a name="position"></a>

!!! variable "Vector2 position"

    Current position in the world of this Actor.
    

### tile<a name="tile"></a>

!!! variable "AtlasTile tile"

    The currently shown tile from the TileSet.
    

### tileSet<a name="tileSet"></a>

!!! variable "std::unique_ptr&lt;TileSet&gt; tileSet"

    The used TileSet for this Actor's sprites.
    

### tileSetSource<a name="tileSetSource"></a>

!!! variable "std::string tileSetSource"

    The path to the TileSet file.
    

## Function Details

### Actor<a name="Actor"></a>
!!! function "Actor(std::string fileName)"

    Constructor that takes a path to the .ractor file.
    

!!! function "Actor(std::unique_ptr&lt;TileSet&gt; tileSet, Vector2 atlasPos, std::string tileSetSource)"

    Constructor that takes a TileSet, the atlas position of the tile to use, and the path to the TileSet.
    

!!! function "Actor(ActorBin bin)"

    Constructor that takes an ActorBin binary structure
    

### addAnimationFrame<a name="addAnimationFrame"></a>
!!! function "void addAnimationFrame(Direction id, Vector2 atlasPos)"

    Add a frame in the chosen animation. The frame represents an atlas tile from this Actor's TileSet.
    

### addAnimationFrames<a name="addAnimationFrames"></a>
!!! function "void addAnimationFrames(Direction id, std::vector&lt;std::vector&lt;int&gt;&gt; frames)"

    Add multiple frames to the chosen animation.
    

### changeAnimation<a name="changeAnimation"></a>
!!! function "void changeAnimation(Direction id)"

    Change the Actor's current animation and play it.
    

### draw<a name="draw"></a>
!!! function "void draw()"

    Draw routine.
    

### dumpJson<a name="dumpJson"></a>
!!! function "json dumpJson()"

    Dump this Actor's data to a nlohmann::json object.
    

### getAnimationRaw<a name="getAnimationRaw"></a>
!!! function "std::vector&lt;Vector2&gt; getAnimationRaw(Direction id)"

    Get a specific animation
    

### getAnimationsRaw<a name="getAnimationsRaw"></a>
!!! function "std::array&lt;std::vector&lt;Vector2&gt;, 8&gt; getAnimationsRaw()"

    Get an array of this Actor's animations
    

### getCollisionRect<a name="getCollisionRect"></a>
!!! function "Rectangle getCollisionRect(Vector2 velocity)"

    Get the collision rectangle of this Actor if it was moved by the velocity vector
    

!!! function "Rectangle getCollisionRect()"

    Get the collision Rectangle of this Actor
    

### getPosition<a name="getPosition"></a>
!!! function "Vector2 getPosition()"

    Get this actor's position in the world.
    

### getRect<a name="getRect"></a>
!!! function "Rectangle getRect()"

    Get the bounding box of this Actor.
    

### getTileSet<a name="getTileSet"></a>
!!! function "TileSet&amp; getTileSet()"

    Get a reference to this Actor's TileSet.
    

### getTileSetSource<a name="getTileSetSource"></a>
!!! function "std::string getTileSetSource()"

    Get the path of the used TileSet.
    

### moveByVelocity<a name="moveByVelocity"></a>
!!! function "void moveByVelocity(Vector2 velocity)"

    Move the Actor using a velocity vector.
    

### removeAnimationFrame<a name="removeAnimationFrame"></a>
!!! function "void removeAnimationFrame(Direction id, int frameIndex)"

    Remove a frame in the chosen animation, specified by an index.
    

### setAnimationFrame<a name="setAnimationFrame"></a>
!!! function "void setAnimationFrame(Direction id, int frameIndex, Vector2 atlasTile)"

    Change a frame in the chosen animation to another atlas tile from the TileSet.
    

### setCollisionRect<a name="setCollisionRect"></a>
!!! function "void setCollisionRect(Rectangle rect)"

    Set the Actor's collision Rectangle
    

### setPosition<a name="setPosition"></a>
!!! function "void setPosition(Vector2 position)"

    Set this Actor's position.
    

### setTileSet<a name="setTileSet"></a>
!!! function "void setTileSet(std::string tileSetSource)"

    Set this Actor's TileSet using a path to the tileset file.
    

### unload<a name="unload"></a>
!!! function "void unload()"

    Unload routine. The UnloadTexture function will called here.
    

### update<a name="update"></a>
!!! function "void update()"

    Update routine.
    

