function interact()
Interface.OpenDialogue(props.dialogue)

room = World.GetRoom()
player = room:GetPlayer()
pos = player:GetPosition()

actor = player:GetActor()

actorpos = actor:GetPosition()

print(actorpos.x)
print(actorpos.y)

print(Direction.LEFT)
actor:PlayAnimation(Direction.LEFT)

end
