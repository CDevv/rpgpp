function interact()
Interface.OpenDialogue(props.dialogue)
GameState.Set("hi", false)

print("hi: " .. tostring(GameState.Get("hi")))
a = 5 / 0
print(a)
--GameState.Get("nonexisty")
vec = Vector2.new(1, 2)
print(vec.x)
print(vec.y)

vec2 = Vector2.new()
print(vec2.x)
print(vec2.y)

room = World.GetRoom()
player = room:GetPlayer()
pos = player:GetPosition()

print(pos.x)
print(pos.y)

player:SetTilePosition(Vector2.new(2, 1))
end
