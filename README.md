# RPG++

![logo](docs/logo.png)

RPG++ is an experimental 2D RPG game engine written in C++. It is currently in early development, but contributions are welcome!

## Building

This project uses [XMake](https://xmake.io/) so you may need to download it for your system.

First, you have to initialize the specified submodules. use `git submodule init` to initialize the paths. Then use, `git submodule update` to clone the submodules to your system.

After cloning the submodules to the paths, check your `/rlImGui` directory.

Additional dependency is LuaJIT. After getting the luajit xmake package, you need to copy the luajit executable into the `execs/` directory.

Here are the xmake directories on different platforms:

Linux: `/home/name/.xmake/packages/l/luajitv2.1.0-beta3/aaeb029456e54f29b904a191c96508d2/bin/`

Windows: `AppData\Local\.xmake\packages\l\luajit\v2.1.0-beta3\7780bd1b1c8146cd984bf13d60e49407\bin`

Mac: `/Users/nikodev/.xmake/packages/l/luajit/v2.1.0-beta3/bin`

(Do not copy it blindly, the commit number might be different)

Finally, use `xmake build --all` to build all targets.

## Running

Use `xmake run editor` to run the editor.

## License

This software uses the MIT License.
