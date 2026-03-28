--- This is a module. Notice that we've declared it as a class
--- and added a `doctype`.
---
--- !doctype module
--- @class meow
local meow = {}

--- Nested namespaces should also be declared as classes.
---
--- !doctype table
--- @class meow.utils
meow.utils = {}

--- Other objects are documented as usual.
function meow.purr() end

--- This class will be included in the documentation
--- for module `meow` because it's located in the namespace `meow`.
---
--- @class meow.MeowingBackend

--- The World Service
---
--- !doctype table
--- @class meow.WorldService
meow.WorldService = {}

--- Get the Player
function meow.WorldService.GetPlayer() end

return meow
