printer()

init_window(640, 480, "RPG++ Game")
init_audio()

g = game.new()
game.init()

game.use_bin("game.bin")

set_fps(60)

while not window_should_close() do
    game.update()
    begin_drawing()
    clear_background()
    game.draw()
    end_drawing()
end

close_window()
close_audio()
