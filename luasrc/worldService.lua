--- The WorldService is responsible for containing and drawing the Room.
---
--- !doctype module
--- @class WorldService
local WorldService = {}

--- Get the Player.
--- @return Player
function WorldService.GetPlayer() end

--- Set the current room using a room title in the game.
--- @param room string
function WorldService.SetRoom(room) end

--- Get the current Room
--- @return Room
function WorldService.GetRoom() end

return WorldService
