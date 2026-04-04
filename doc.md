# Actor

## GetPosition


```lua
function Actor.GetPosition()
  -> Vector2
```


---

# GameState

 The StateService is responsible for storing gameplay-related variables
 that make up the state of the game.

 !doctype module

## Get


```lua
function GameState.Get(prop: any)
  -> any
```

 Get a property

## Set


```lua
function GameState.Set(prop: any, value: any)
```

 Set the property with the chosen value.


---

# Interface

 The InterfaceService acts for the User Interface (UI).

 !doctype module

## OpenDialogue


```lua
function Interface.OpenDialogue(dialogue: string)
```

 Open a dialogue with a certain title.

## test


```lua
function Interface.test()
```

 hello.


---

# LuaLS


---

# Sounds

 The SoundService is responsible for playing sounds and managing the current background music

 !doctype module

## LoadMusic


```lua
function Sounds.LoadMusic(music: string)
```

 Load the music with the specified id.

## PlayMusic


```lua
function Sounds.PlayMusic()
```

 Play the loaded music.

## PlaySound


```lua
function Sounds.PlaySound(sound: string)
```

 Play the sound with the specified id.


---

# Vector2

## new


```lua
function Vector2.new(x: any, y: any)
```

 Construct a new `Vector2`

## x


```lua
integer
```

## y


```lua
integer
```


---

# WorldService

 The WorldService is responsible for containing and drawing the Room.

 !doctype module

## GetPlayer


```lua
function WorldService.GetPlayer()
  -> Player
```

 Get the Player.

## GetRoom


```lua
function WorldService.GetRoom()
  -> Room
```

 Get the current Room

## SetRoom


```lua
function WorldService.SetRoom(room: string)
```

 Set the current room using a room title in the game.


---

# dir