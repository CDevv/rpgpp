package("raylib")
    set_sourcedir(path.join(os.scriptdir(), "libs/raylib/src"))

    if is_plat("mingw", "linux", "macosx") then
        add_syslinks("raylib")
    end

    if is_plat("mingw", "windows") then
        add_syslinks("gdi32", "opengl32", "winmm", "shell32", "user32")
    end

    on_install("linux", "macosx", "mingw", "windows", function (package)
    if is_plat("windows") then
        os.cd(path.join(os.scriptdir(), "libs/raylib/"))
        import("package.tools.cmake").install(package, {})
        elseif is_plat("mingw") then
            import("package.tools.make").make(package, {})
            os.cp(path.join(os.scriptdir(), "libs/raylib/src/libraylib.a"),
                    path.join(os.scriptdir(), "libs/libraylib.a"))
        else
            os.cp(path.join(os.scriptdir(), "libs/SDL2"),
                    path.join(os.scriptdir(), "libs/raylib/src/external/SDL2"))
            import("package.tools.make").make(package, {"PLATFORM=PLATFORM_DESKTOP_SDL"})
            os.cp(path.join(os.scriptdir(), "libs/raylib/src/libraylib.a"),
            path.join(os.scriptdir(), "libs/libraylib.a"))
        end
    end)
package_end()

add_requires("raylib", "libsdl2", "nlohmann_json", "nativefiledialog-extended", "alpaca", "reproc")
add_rules("mode.debug")
set_defaultmode("debug")

task("resources")
    on_run(function()
       os.cp("$(curdir)/resources", "$(builddir)/$(plat)/$(arch)/$(mode)/resources")
    end)

target("rpgpp")
    set_kind("static")
	set_languages("cxx17")
    add_includedirs("include/")
    add_linkdirs("libs/")
    add_files("src/*.cpp")
    add_packages("raylib", "nlohmann_json", "alpaca")
    if is_plat("linux", "macosx") then
        add_packages("libsdl2", {public = true})
    end
    if is_plat("linux") then
        add_cxxflags("-fPIC")
    end

target("rpgpplua")
    set_kind("shared")
	set_languages("cxx17")
    add_includedirs("include/", "libs/luajit/src/")
    add_files("src/rpgpplua/*.cpp")
    add_packages("nlohmann_json", "raylib", "alpaca", {public = true})
    add_linkdirs("libs/")
    add_links("luajit")
	add_deps("rpgpp")

target("editor")
    set_kind("binary")
	set_languages("cxx17")
    add_includedirs("include/", "include/editor/", "include/imgui/", "libs/raylib/src/")
    add_files("src/editor/*.cpp")
    add_packages("raylib", "nlohmann_json", "nativefiledialog-extended", "reproc")
	add_links("luajit", "rlImGui")
    add_linkdirs("libs/")
    add_deps("rpgpp")
    after_build(function ()
        import("core.project.task")
        task.run("resources")
    end)

target("game")
    set_kind("binary")
	set_languages("cxx17")
    add_includedirs("include/", "libs/raylib/src/")
    add_files("src/game/*.cpp")
    add_packages("raylib", "nlohmann_json")
    add_linkdirs("libs/")
	add_links("luajit")
    add_deps("rpgpp")

