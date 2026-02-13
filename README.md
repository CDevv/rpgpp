![logo](docs/logo.png)
---

RPG++ is an experimental 2D RPG game engine written in C++. It is currently in early development, but contributions are welcome!

***This is a fresh restart, there are currently no RPG++ lua bindings, the engine hasn't been implemented in this branch yet.***

Requirements
---

To build RPG++, you'll need to install the following tools/packages:
All platforms:
- [xmake](https://xmake.io/) (follow the instructions on the website to install XMake on your system)
- [git](https://git-scm.com/)
- [tree-sitter CLI](https://github.com/tree-sitter/tree-sitter/blob/master/crates/cli/README.md)
  - Note that you will need cargo or npm to install the CLI.
Specifically for Linux:
- gcc, g++, make, and ninja (on Debian-based distros, you can install this via `sudo apt install build-essential`)
- libx11-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev, libxi-dev, libgl1-mesa-dev, and mesa-common-dev (on Debian-based distros, you can install this via `sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev mesa-common-dev`)
Specifically for Windows:
- Visual Studio 2019 or later with Desktop development with C++

Building
---

Clone the project using git with

```bash
git clone https://github.com/CDevv/rpgpp
```

Then, to build all targets, run

```bash
xmake build --all
```

If xmake failed to build, try force reinstalling the packages with
```bash
xmake clean --all # remove build artifacts
xmake require --clean # remove installed package cache for project
xmake require --force # force install of package
```

Running
---

To start the editor, run

```bash
xmake run editor
```

License
---

This software uses the MIT License.
