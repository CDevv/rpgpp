# RPG++

![logo](docs/logo.png)

RPG++ is an experimental 2D RPG game engine written in C++. It is currently in early development, but contributions are welcome!

## Building

This project uses [XMake](https://xmake.io/) so you may need to download it for your system.

First, you have to initialize the specified submodules. use `git submodule init` to initialize the paths. Then use, `git submodule update` to clone the submodules to your system.

After cloning the submodules to the paths, check your `/rlImGui` directory. Use `premake5.exe` on Windows or `premake5` on Linux to clone to raylib and imgui repositories to rlImGui.

Finally, use `xmake build --all` to build all targets.

## Running

Use `xmake run editor` to run the editor.

## License

This software uses the MIT License.
