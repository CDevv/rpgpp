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

Then, to build all targets, run

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
