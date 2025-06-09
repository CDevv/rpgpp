add_requires("raylib", "nlohmann_json")

target("rpgpp")
    set_kind("binary")
    add_includedirs("include/")
    add_files("src/*.cpp")
    add_packages("raylib", "nlohmann_json")
    set_targetdir("./")

