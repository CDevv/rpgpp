# RPG++

![logo](docs/logo.png)

RPG++ is an experimental 2D RPG game engine written in C++. It is currently in early development, but contributions are welcome!

## Requirements
To build RPG++, you'll need to install the following tools/packages:
 - [XMake](https://xmake.io/) (follow the instructions on the website to install XMake on your system)
 - gcc, g++, make, ninja (on Debian-based distros, you can install this via `sudo apt install build-essential`)
 - [git](https://git-scm.com/)

## Building
Clone the project using git with
```
git clone https://github.com/CDevv/rpgpp
```
This project uses submodules from other repositories for it's libraries. You will need to initialize the submodule paths using
```
git submodule init
```
After that, clone/update the submodules to the project using
```
git submodule update
```

(deprecated/should_remove) After cloning the submodules to the paths, check your `/rlImGui` directory.

This project uses LuaJIT as one of it's dependecies.
After getting the luajit xmake package, you will need to copy the luajit executable into the `execs/` directory.

Here are the xmake directories on different platforms:
- Linux: `/home/name/.xmake/packages/l/luajit/v2.1.0-beta3/aaeb029456e54f29b904a191c96508d2/bin/`
- Windows: `AppData\Local\.xmake\packages\l\luajit\v2.1.0-beta3\7780bd1b1c8146cd984bf13d60e49407\bin`
- Mac: `/Users/nikodev/.xmake/packages/l/luajit/v2.1.0-beta3/bin`

(Do not copy it blindly, the commit number might be different)

Finally, to build all targets, run
```
xmake build --all
```

## Running
To start the editor, run
```
xmake run editor
```

## License

This software uses the MIT License.
