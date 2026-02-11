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

Another dependency of this project is TGUI (stable release).
The TGUI library that xmake built may not work when linking. In this case, you can attempt to build it using CMake. The output should be tgui-s-d.lib on Windows.

Finally, to build all targets, run

```bash
xmake build --all
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
