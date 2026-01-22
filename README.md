![logo](docs/logo.png)
---

RPG++ is an experimental 2D RPG game engine written in C++. It is currently in early development, but contributions are welcome!

***This is a fresh restart, there are currently no RPG++ lua bindings, the engine hasn't been implemented in this branch yet.***

Requirements
---

To build RPG++, you'll need to install the following tools/packages:

- [xmake](https://xmake.io/) (follow the instructions on the website to install XMake on your system)
- gcc, g++, make, ninja (on Debian-based distros, you can install this via `sudo apt install build-essential`)
- [git](https://git-scm.com/)

Building
---

Clone the project using git with

```bash
git clone https://github.com/CDevv/rpgpp
```

This project uses submodules from other repositories for it's libraries. You will need to initialize the submodule paths using

```bash
git submodule init
```

After that, clone/update the submodules to the project using

```bash
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

Another dependency of this project is TGUI (stable release).
The TGUI library that xmake built may not work when linking. In this case, you can attempt to build it using CMake. The output should be tgui-s-d.lib on Windows.

Finally, to build all targets, run

```bash
xmake build --all
```

After building, the `resourcesDiff` and the `resources` folder will be copied to the build directory (the `resourcesDiff` folder's contents will be directly copied to the root of your build directory). The contents inside the `resourcesDiff` folder will only be copied if you make changes.

Running
---

To start the editor, run

```bash
xmake run editor
```

License
---

This software uses the MIT License.
