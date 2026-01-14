package("raylib")
set_sourcedir(path.join(os.scriptdir(), "libs/raylib/src"))

if is_plat("mingw", "linux", "macosx") then
    add_syslinks("raylib")
end

if is_plat("mingw", "windows") then
    add_syslinks("gdi32", "opengl32", "winmm", "shell32", "user32")
end

on_install("linux", "macosx", "mingw", "windows", function(package)
    os.cd(path.join(os.scriptdir(), "libs/raylib/"))
    import("package.tools.cmake").install(package, {})
end)
package_end()

package("tgui")
set_sourcedir(path.join(os.scriptdir(), "libs/tgui/"))
add_deps("cmake", "raylib")
add_extsources("raylib")
on_install("linux", "macosx", "mingw", "windows", function(package)
    local configs = {}
    table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
    table.insert(configs, "-DBUILD_SHARED_LIBS=OFF")
    table.insert(configs, "-DTGUI_BACKEND=RAYLIB")
    table.insert(configs, "-DTGUI_BUILD_GUI_BUILDER=OFF")
    import("package.tools.cmake").install(package, configs)
end)
package_end()

add_requires("raylib", "tgui", "nlohmann_json", "nativefiledialog-extended", "reproc", "luajit")
add_rules("mode.debug")
set_defaultmode("debug")

task("resources")
on_run(function()
    os.cp("$(curdir)/resources", "$(builddir)/$(plat)/$(arch)/$(mode)/", { async = true })
    os.cp("$(curdir)/execs", "$(builddir)/$(plat)/$(arch)/$(mode)/", { async = true })
    if is_plat("linux", "macosx") then
        os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/librpgpp.a", "$(curdir)/game-src/lib/librpgpp.a", { async = true })
        os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/librpgpplua.so", "$(curdir)/game-src/lib/librpgpplua.so",
            { async = true })
    end
    if is_plat("windows") then
        os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/rpgpplua.lib", "$(curdir)/game-src/lib/rpgpplua.lib", { async = true })
        os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/rpgpplua.dll", "$(curdir)/game-src/lib/rpgpplua.dll", { async = true })
    end
    os.cp("$(curdir)/game-src", "$(builddir)/$(plat)/$(arch)/$(mode)/", { async = true })
end)

target("rlImGui")
    set_kind("static")
    set_languages("cxx17")
    add_includedirs("rlImGui/", "rlImGui/imgui-master/", "rlImGui/extras/", "include/raylib/")
    add_files("rlImGui/*.cpp", "rlImGui/imgui-master/*.cpp")
    add_packages("raylib")

target("rpgpp")
    set_kind("static")
    set_languages("cxx17")
    add_includedirs("include/", "include/luajit/")
    add_linkdirs("libs/")
    add_links("luajit")
    add_files("src/*.cpp")
    add_packages("raylib", "nlohmann_json")
    if is_plat("linux") then
        add_cxxflags("-fPIC")
    end

target("rpgpplua")
    set_kind("shared")
    set_languages("cxx17")
    add_includedirs("include/", "libs/luajit/src/", "include/luajit/")
    add_files("src/rpgpplua/*.cpp")
    add_packages("nlohmann_json", "raylib", "luajit", { public = true })
    add_linkdirs("libs/")
    add_deps("rpgpp")
--  add_links("luajit")
--  there were some linking issues with MSVC, removed for testing. fuck msvc.

target("editor")
    -- DO NOT REMOVE: /s
    if is_plat("windows") then
        add_defines("TGUI_STATIC")
    end
    set_kind("binary")
    set_languages("cxx17")
    add_includedirs("include/", "include/editor/", "include/imgui/", "libs/raylib/src/", "rlImGui/", "libs/tgui/include/")
    add_files("src/editor/*.cpp", "src/editor/**/*.cpp")
    add_deps("rpgpp", "rlImGui")
    add_packages("raylib", "tgui", "nlohmann_json", "nativefiledialog-extended", "reproc", "luajit")
    add_linkdirs("libs/")
    after_build(function()
        import("core.project.task")
        task.run("resources")
    end)

-- UNTIL MSVC FIGURES ITSELF OUT
-- THIS SHALL STAY
--target("game")
--set_kind("binary")
--set_languages("cxx17")
--add_includedirs("include/", "libs/raylib/src/")
---add_files("src/game/*.cpp")
--add_packages("raylib", "nlohmann_json", "luajit")
--add_linkdirs("libs/")
--add_deps("rpgpp")
