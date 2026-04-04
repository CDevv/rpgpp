--- The StateService is responsible for storing gameplay-related variables
--- that make up the state of the game.
---
--- @class GameState
local GameState = {}

--- Get a property
--- @param prop string
--- @return any
function GameState.Get(prop) end

--- Set the property with the chosen value.
--- @param prop string
--- @param value any
function GameState.Set(prop, value) end

return GameState