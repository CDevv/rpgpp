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

add_requires("raylib", "libsdl2", "nlohmann_json", "nativefiledialog-extended", "alpaca", "reproc", "luajit")
add_rules("mode.debug")
set_defaultmode("debug")

final_build = "$(builddir)/$(plat)/$(arch)/$(mode)"

task("resources")
    on_run(function()
       os.cp("$(curdir)/resources", "$(builddir)/$(plat)/$(arch)/$(mode)/")
       os.cp("$(curdir)/execs", "$(builddir)/$(plat)/$(arch)/$(mode)/")
       if is_plat("linux", "macosx") then
           os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/librpgpp.a", "$(curdir)/game-src/lib/librpgpp.a")
           os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/librpgpplua.so", "$(curdir)/game-src/lib/librpgpplua.so")
       end
       if is_plat("windows") then
           os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/rpgpplua.lib", "$(curdir)/game-src/lib/rpgpplua.lib")
           os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/rpgpplua.dll", "$(curdir)/game-src/lib/rpgpplua.dll")
       end
       os.cp("$(curdir)/game-src", "$(builddir)/$(plat)/$(arch)/$(mode)/")
    end)

target("rlImGui")
    set_kind("static")
    set_languages("cxx17")
    add_includedirs("rlImGui/", "rlImGui/imgui-master/", "rlImGui/extras/")
    add_files("rlImGui/*.cpp", "rlImGui/imgui-master/*.cpp")
    add_packages("raylib")

target("rpgpp")
    set_kind("static")
    set_languages("cxx17")
    add_includedirs("include/", "include/luajit/")
    add_linkdirs("libs/")
    add_links("luajit")
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
    add_includedirs("include/", "libs/luajit/src/", "include/luajit/")
    add_files("src/rpgpplua/*.cpp")
    add_packages("nlohmann_json", "raylib", "alpaca", "luajit", {public = true})
    add_linkdirs("libs/")
    add_deps("rpgpp")
    -- add_links("luajit")
    -- there were some linking issues with MSVC, removed for testing

target("editor")
    set_kind("binary")
    set_languages("cxx17")
    add_includedirs("include/", "include/editor/", "include/imgui/", "libs/raylib/src/", "rlImGui/")
    add_files("src/editor/*.cpp")
    --add_deps("rpgpp", "rlImGui")
    add_deps("rpgpp")
    add_links("rlImGui")
    add_packages("raylib", "nlohmann_json", "nativefiledialog-extended", "reproc", "luajit")
    add_linkdirs("libs/")
    after_build(function ()
        import("core.project.task")
        task.run("resources")
    end)

target("game")
    set_kind("binary")
	set_languages("cxx17")
    add_includedirs("include/", "libs/raylib/src/")
    add_files("src/game/*.cpp")
    add_packages("raylib", "nlohmann_json", "luajit")
    add_linkdirs("libs/")
    add_deps("rpgpp")
