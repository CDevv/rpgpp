--- The SoundService is responsible for playing sounds and managing the current background music
---
--- !doctype module
--- @class Sounds
local Sounds = {}

--- Play the sound with the specified id.
--- @param sound string
function Sounds.PlaySound(sound) end

--- Load the music with the specified id.
--- @param music string
function Sounds.LoadMusic(music) end

--- Play the loaded music.
function Sounds.PlayMusic() end

return Sounds