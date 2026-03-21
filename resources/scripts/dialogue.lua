function interact()
	Interface.OpenDialogue(props.dialogue)

	room = World.GetRoom()
	player = room:GetPlayer()
	pos = player:GetPosition()

	actor = player:GetActor()

	actorpos = actor:GetPosition()

	print(actorpos.x)
	print(actorpos.y)

	GameState.Set("test_nil", nil);
	GameState.Set("test_bool", true);
	GameState.Set("test_int1", 50);
	GameState.Set("test_int2", 4611686018427387904);
	GameState.Set("test_double", 3.14);
	GameState.Set("test_string", "hello");
	GameState.Set("test_table1", { 1, 2, "test" });
	GameState.Set("test_table2", { key = "test" });
	GameState.Set("test_table3", { key = "test", 2, 4 });

	print(GameState.Get("test_nil"), type(GameState.Get("test_nil")))
	print(GameState.Get("test_bool"), type(GameState.Get("test_bool")))
	print(GameState.Get("test_int1"), type(GameState.Get("test_int1")))
	print(GameState.Get("test_int2"), type(GameState.Get("test_int2")))
	print(GameState.Get("test_double"), type(GameState.Get("test_double")))
	print(GameState.Get("test_string"), type(GameState.Get("test_string")))
	print(GameState.Get("test_table1"), type(GameState.Get("test_table1")))
	print(GameState.Get("test_table2"), type(GameState.Get("test_table2")))
	print(GameState.Get("test_table3"), type(GameState.Get("test_table3")))

	print(Direction.LEFT)
	actor:PlayAnimation(Direction.LEFT)
end
