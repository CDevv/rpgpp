---
generator: doxide
---


# Room

**class Room**

This class represents a game's room. It can contain the Player, interactables, collisions and the TileMap


## Variables

| Name | Description |
| ---- | ----------- |
| [interactables](#interactables) | Container of the interactable tiles  |
| [collisions](#collisions) | Container of the collision tiles  |
| [tileMap](#tileMap) | This Room's TileMap, which contains all placed tiles. |
| [actors](#actors) | A collection of all Actors in this Room  |
| [player](#player) | This Room's only Player. |

## Functions

| Name | Description |
| ---- | ----------- |
| [Room](#Room) | Empty constructor  |
| [Room](#Room) | Construct a Room from an .rmap file  |
| [Room](#Room) | Construct a Room by using a TileMap pointer  |
| [Room](#Room) | Construct a Room by using the RoomBin binary structure :material-location-enter: `bin` :    The binary structure to use  |
| [dumpJson](#dumpJson) |     Dump the room's data into a nlohmann::json object for later use Useful for dumping into a .json file. :material-location-exit: **Return** :    The JSON object  |
| [unload](#unload) |     Unload used resources. |
| [update](#update) |     Update routine for this Room. |
| [draw](#draw) |     Draw routine for this Room. |
| [addActor](#addActor) |     Add an actor to this Room :material-location-enter: `actor` :    The actor to be added to the Room's collection.  |
| [addPlayer](#addPlayer) |     Add the Player into this Room. |
| [getPlayer](#getPlayer) |     Get a reference to the Player object. |
| [getTileMap](#getTileMap) |     Get a pointer to this Room's TileMap. |
| [getCollisions](#getCollisions) |     Get a reference to the CollisionsContainer of this Room. |
| [getInteractables](#getInteractables) |     Get a reference to the InteractablesContainer of this Room. |

## Variable Details

### actors<a name="actors"></a>

!!! variable "std::unique_ptr&lt;std::vector&lt;Actor&gt;&gt; actors"

    A collection of all Actors in this Room
    

### collisions<a name="collisions"></a>

!!! variable "std::unique_ptr&lt;CollisionsContainer&gt; collisions"

    Container of the collision tiles
    

### interactables<a name="interactables"></a>

!!! variable "std::unique_ptr&lt;InteractablesContainer&gt; interactables"

    Container of the interactable tiles
    

### player<a name="player"></a>

!!! variable "std::unique_ptr&lt;Player&gt; player"

    This Room's only Player.
    

### tileMap<a name="tileMap"></a>

!!! variable "std::unique_ptr&lt;TileMap&gt; tileMap"

    This Room's TileMap, which contains all placed tiles.
    

## Function Details

### Room<a name="Room"></a>
!!! function "Room()"

    Empty constructor
    

!!! function "Room(std::string fileName)"

    Construct a Room from an .rmap file
    

!!! function "Room(std::unique_ptr&lt;TileMap&gt; tileMap)"

    Construct a Room by using a TileMap pointer
    

!!! function "Room(RoomBin bin)"

    Construct a Room by using the RoomBin binary structure
    
    :material-location-enter: `bin`
    :    The binary structure to use
    

### addActor<a name="addActor"></a>
!!! function "void addActor(Actor actor)"

        Add an actor to this Room
        
    :material-location-enter: `actor`
    :    The actor to be added to the Room's collection.
    

### addPlayer<a name="addPlayer"></a>
!!! function "void addPlayer(std::unique_ptr&lt;Player&gt; player)"

        Add the Player into this Room. A unique_ptr is passed whose ownership will be moved into the Room.
        
    :material-location-enter: `player`
    :    A pointer to the player that will be moved into this Room.
    

### draw<a name="draw"></a>
!!! function "void draw()"

        Draw routine for this Room.
    

### dumpJson<a name="dumpJson"></a>
!!! function "json dumpJson()"

        Dump the room's data into a nlohmann::json object for later use
        Useful for dumping into a .json file.
        
    :material-location-exit: **Return**
    :    The JSON object
    

### getCollisions<a name="getCollisions"></a>
!!! function "CollisionsContainer&amp; getCollisions()"

        Get a reference to the CollisionsContainer of this Room.
    

### getInteractables<a name="getInteractables"></a>
!!! function "InteractablesContainer&amp; getInteractables()"

        Get a reference to the InteractablesContainer of this Room.
    

### getPlayer<a name="getPlayer"></a>
!!! function "Player&amp; getPlayer()"

        Get a reference to the Player object.
        
    :material-location-exit: **Return**
    :    The Player in this Room.
    

### getTileMap<a name="getTileMap"></a>
!!! function "TileMap &#42;getTileMap()"

        Get a pointer to this Room's TileMap.
        
    :material-location-exit: **Return**
    :    A pointer to the room's TileMap.
    

### unload<a name="unload"></a>
!!! function "void unload()"

        Unload used resources. Typically unloads used Textures by calling raylib's UnloadTexture
    

### update<a name="update"></a>
!!! function "void update()"

        Update routine for this Room.
    

