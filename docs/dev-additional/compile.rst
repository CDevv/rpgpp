Compiling RPG++
===============

====================
Getting dependencies
====================

To build RPG++, you will need the following tools and packages:

- **For all platforms**:

  - `xmake <https://xmake.io/>`__ (follow the instructions on the website to install XMake on your system)
  - `git <https://git-scm.com/>`__

- Specifically for **Linux** (instructions to install packages are listed below):

  - Packages: ``gcc``, ``g++``, ``make``, ``ninja``, ``libx11-dev``, ``libxrandr-dev``, ``libxinerama-dev``, ``libxcursor-dev``, ``libxi-dev``, ``libgl1-mesa-dev``, and ``mesa-common-dev``

  - Debian:

    .. code-block:: bash

       sudo apt install build-essential ninja-build libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev mesa-common-dev

  - Fedora:

    .. code-block:: bash

       sudo dnf install gcc gcc-c++ make ninja-build libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel mesa-libGL-devel

  - Arch Linux:

    .. code-block:: bash

       sudo pacman -S base-devel ninja libx11 libxrandr libxinerama libxcursor libxi mesa

- Specifically for **Windows**:

  - Visual Studio 2019 or later with Desktop development with C++ (community edition will suffice)

- Specifically for **MacOS**:

  - Xcode

========
Building
========

1. Clone the project using git with

``git clone https://github.com/rpgppengine/rpgpp.git``

2. Build all targets by running the following commands (follow the instructions to install packages if there is one)

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
