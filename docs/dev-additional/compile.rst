Compiling RPG++
===============

====================
Getting dependencies
====================

To build RPG++, you'll need to install the following tools/packages:

- **For all platforms**:

  - `xmake <https://xmake.io/>`__ (follow the instructions on the website to install XMake on your system)
  - `git <https://git-scm.com/>`__

- Specifically for **Linux**:

  - gcc, g++, make, and ninja (on Debian-based distros, you can install this via `sudo apt install build-essential`)
  - libx11-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev, libxi-dev, libgl1-mesa-dev, and mesa-common-dev (Debian-based distros can install this via `sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev mesa-common-dev`)

- Specifically for **Windows**:

  - Visual Studio 2019 or later with Desktop development with C++ (community edition will suffice)

- Specifically for **MacOS**:

  - Xcode

========
Building
========

1. Clone the project using git with

``git clone https://github.com/rpgppengine/rpgpp.git``

2. Build all targets by running and following the instructions if there's one

``xmake build --all``

.. warning:: If build fails.
    You can try the following:

    ::

        xmake clean --all # remove build artifacts
        xmake require --clean # remove installed package cache for project
        xmake require --force # force install of package

=======
Running
=======

You can run the RPG++ Editor with:

``xmake run editor``