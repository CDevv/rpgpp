![logo](readme/logo.png)
---

[![Build RPG++](https://github.com/rpgppengine/rpgpp/actions/workflows/build.yml/badge.svg)](https://github.com/rpgppengine/rpgpp/actions/workflows/build.yml)
[![Build documentations](https://github.com/rpgppengine/rpgpp/actions/workflows/build-doc.yml/badge.svg)](https://github.com/rpgppengine/rpgpp/actions/workflows/build-doc.yml)
[![CodeQL Advanced](https://github.com/rpgppengine/rpgpp/actions/workflows/codeql.yml/badge.svg)](https://github.com/rpgppengine/rpgpp/actions/workflows/codeql.yml)

RPG++ is an experimental 2D RPG game engine and editor written in C++. It is currently in early development, but contributions are welcome!

<img src="readme/readme_img1.png" alt="screenshot of engine" width="640">

Requirements
---
To build RPG++, you will need the following tools and packages:
- **For all platforms**:
  - [xmake](https://xmake.io/) (follow the instructions on the website to install XMake on your system)
  - [git](https://git-scm.com/)
- Specifically for _Linux_ (instructions to install packages are listed below):
  - Packages: `gcc`, `g++`, `make`, `ninja`, `libx11-dev`, `libxrandr-dev`, `libxinerama-dev`, `libxcursor-dev`, `libxi-dev`, `libgl1-mesa-dev`, and `mesa-common-dev`
  - Debian:
	```bash
	sudo apt install build-essential ninja-build libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev mesa-common-dev
	```
  - Fedora:
	```bash
	sudo dnf install gcc gcc-c++ make ninja-build libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel mesa-libGL-devel
	```
  - Arch Linux:
	```bash
	sudo pacman -S base-devel ninja libx11 libxrandr libxinerama libxcursor libxi mesa
	```
- Specifically for _Windows_:
  - Visual Studio 2019 or later with Desktop development with C++ (community edition will suffice)
- Specifically for _MacOS_:
  - Xcode

*Optionally*, if you like to contribute and build the documentations for RPG++, you will need the following tools and packages. Note that the building of documentations is not required to build the editor, and vice versa:
- [Python version 3.12 or higher](https://www.python.org/) (to install Python, follow the specific instruction for your OS)
- [doxygen](https://www.doxygen.nl/download.html)

Building the RPG++ engine/editor
---

1. Clone the project using git with

```bash
git clone https://github.com/rpgppengine/rpgpp.git
```

2. Build all targets by running the following commands (follow the instructions to install packages if there is one)

```bash
xmake build --all
```

_If xmake failed to build, try force reinstalling the packages with_
```bash
xmake clean --all # remove build artifacts
xmake require --clean # remove installed package cache for project
xmake require --force # force install of package
```

Building the documentations
---
1. Create a Python environment if you haven't created one with
```bash
python -m venv .venv # you may need to specify a different command like "python3" if you have multiple versions of Python
```

2. Activate the environment with one of the following commands
```bash
# For Windows
.venv\Scripts\activate.bat # cmd
.venv\Scripts\Activate.ps1 # powershell
# For Linux/MacOS
source .venv/bin/activate # bash/zsh
source .venv/bin/activate.fish # fish
source .venv/bin/activate.csh # csh/tcsh
```

3. Install the necessary packages with
```bash
pip install -r requirements.txt
```
4. Generate and build the docs with
```bash
xmake build_doc
```
or optionally, run each of the commands below __if the command above fails__:
```bash
mkdir -p build/
doxygen doxygen.conf
breathe-apidoc -o docs/Dev -m -f build/doxygen/xml
make html
```

You should see the docs output at `build/html`.

Running the editor
---

To start the RPG++ editor, run

```bash
xmake run editor
```

Serving the documentation
---

To serve the documentation, you can use any server hosting software. Make sure that the software is set to host at `build/html` within the repo.

For example, to serve the documentation with Python's builtin `http.server`, run the following command within the root of the repo:
```bash
python -m http.server --directory build/html
```

Translation
---

Translation are located in `resources/translations`, containing JSON files with their names being the language code.

Each JSON files is structured as either flattened JSON:
```json
{
	"language": "<name_of_language>",
	"translation.key": "<translation_value>"
}
```
or nested JSON:
```json
{
	"language": "<name_of_language>",
	"translation": {
		"key": "<translation_value>"
	}
}
```
Both types are supported, though we recommend using nested JSON for better organization.

If you wish to improve an existing language, you can do so by modifying the language file of the language you know. Or if you want to add a new language, create a new JSON file within the folder, specify the language via the `language` key, and then translate the root translation file (en_us.json). The engine will automatically pick it up.

To check for translation progress, you can do so via
```
xmake check_translation
```

License
---

This software is licensed under the MIT License.
