package("raylib")
-- set_sourcedir(path.join(os.scriptdir(), "libs/raylib/src"))
add_urls("https://github.com/raysan5/raylib.git")
add_versions("5.5.99", "13f9112d8c069ed333acf72c2c1b94a0533c6dc1")
add_deps("cmake")
set_license("Zlib")

if is_plat("mingw", "linux", "macosx") then
    add_syslinks("raylib")
end

if is_plat("mingw", "windows") then
    add_syslinks("gdi32", "opengl32", "winmm", "shell32", "user32")
end

on_install("linux", "macosx", "mingw", "windows", function(package)
    -- os.cd(path.join(os.scriptdir(), "libs/raylib/"))
    import("package.tools.cmake").install(package, {})
end)
package_end()

-- TODO: The build tool for tree-sitter-lua requires TREE_SITTER_CLI.
-- Could either compile it ourself or ask user to install it outside.
-- For now, this package definition has been skipped.
package("tree-sitter-lua")
add_urls("https://github.com/tree-sitter-grammars/tree-sitter-lua.git")
add_versions("0.4.99", "e40f5b6e6df9c2d1d6d664ff5d346a75d71ee6b2")
add_deps("make")
set_license("MIT")
on_install("linux", "macosx", function(package)
    local config = {}
    table.insert(config, "install")
    table.insert(config, "PREFIX=" .. package:installdir())
    import("package.tools.make").make(package, config)
    -- This is a very fucky hack to force the linker to only use the static library.
    -- But hey, it works!
    os.rm(path.join(package:installdir(), "lib/*.so"), { async = true })
    os.rm(path.join(package:installdir(), "lib/*.so.*"), { async = true })
end)
-- This is another very fucky hack, since for some reason, make works on Linux but not cmake, while cmake works on Windows but not make.
-- ... don't we all love Windows???
on_install("mingw", "windows", function(package)
    local config = {}
    table.insert(config, "-DCMAKE_BUILD_TYPE=" .. (is_mode("debug") and "Debug" or "Release"))
    table.insert(config, "-DBUILD_SHARED_LIBS=OFF")
    import("package.tools.cmake").install(package, config)
end)
package_end()

package("tgui")
-- set_sourcedir(path.join(os.scriptdir(), "libs/tgui/"))
add_urls("https://github.com/texus/TGUI.git")
add_versions("1.12.99", "c6d138509f2aae33cbabb09a91c5857eba990657")
add_deps("cmake", "raylib")
set_license("Zlib")
add_extsources("raylib")
on_install("linux", "macosx", "mingw", "windows", function(package)
    local configs = {}
    table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (is_mode("debug") and "Debug" or "Release"))
    table.insert(configs, "-DBUILD_SHARED_LIBS=OFF")
    table.insert(configs, "-DTGUI_BACKEND=RAYLIB")
    table.insert(configs, "-DTGUI_BUILD_GUI_BUILDER=OFF")
    import("package.tools.cmake").install(package, configs)
end)
package_end()

add_requires("raylib", "tgui", "nlohmann_json", "nativefiledialog-extended", "reproc", "luajit", "tree-sitter",
    "tree-sitter-lua")
add_rules("mode.debug", "mode.release")
set_defaultmode("debug")

target("rpgpp")
set_kind("static")
set_languages("cxx17")
add_includedirs("include/") --, "libs/raylib/src")
add_files("src/*.cpp")
add_packages("raylib", "nlohmann_json", "luajit")
if is_plat("linux") then
    add_cxxflags("-fPIC")
end

-- !!!!!!! FIX MSVC ISSUE !!!!!!!
--
-- ATTENTION! The RPG++ LUA SOURCES ARE NOT INCLUDED YET. THIS IS CAUSING LINKER ISSUES!
-- ADD THEM TO THE PROJECT, THEN UNCOMMENT THIS PART!
--
target("rpgpplua")
set_kind("shared")
set_languages("cxx17")
add_includedirs("include/")
add_files("src/rpgpplua/*.cpp")
add_packages("nlohmann_json", "raylib", "luajit", { public = true })
add_deps("rpgpp")

target("editor")
-- add_includedirs("include/", "include/editor/", "libs/raylib/src/", "libs/tgui/include/")
-- add_includedirs("libs/raylib/src/", "libs/tgui/include/")
if is_plat("windows") then
    add_defines("TGUI_STATIC")
    -- Add the actual reference to this file, if necessary.
    -- Since it doesn't work on my environment, it was removed.
    -- If you need to add it, uncomment this part!
    --
    -- add_links("tgui-s-d")
end
-- add_links("tree-sitter-lua", { kind = "static" })
set_kind("binary")
set_languages("cxx17")
add_includedirs("include/", "include/editor/", os.dirs(path.join(os.scriptdir(), "include/editor/**")))
add_files("src/editor/**.cpp")
add_deps("rpgpp")
add_packages("raylib", "tgui", "nlohmann_json", "nativefiledialog-extended", "reproc", "luajit", "tree-sitter",
    "tree-sitter-lua")
after_build(function(target)
    os.cp("$(curdir)/resources", "$(builddir)/$(plat)/$(arch)/$(mode)/", { async = true })
    if is_plat("linux", "macosx") then
        os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/librpgpp.a", "$(curdir)/game-src/lib/librpgpp.a", { async = true })
        os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/librpgpplua.so", "$(curdir)/game-src/lib/librpgpplua.so",
            { async = true })
        os.cp(path.join(target:pkg("luajit"):installdir(), "bin/luajit*"), "$(curdir)/execs/luajit", { async = true })
    end
    if is_plat("windows") then
        os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/rpgpplua.lib", "$(curdir)/game-src/lib/rpgpplua.lib", { async = true })
        os.cp("$(builddir)/$(plat)/$(arch)/$(mode)/rpgpplua.dll", "$(curdir)/game-src/lib/rpgpplua.dll", { async = true })
        os.cp(path.join(target:pkg("luajit"):installdir(), "bin/luajit.exe"), "$(curdir)/execs/luajit.exe",
            { async = true })
    end
    os.cp("$(curdir)/game-src", "$(builddir)/$(plat)/$(arch)/$(mode)/", { async = true })
    os.cp("$(curdir)/execs", "$(builddir)/$(plat)/$(arch)/$(mode)/", { async = true })

    os.rm("$(curdir)/game-src", { async = true })
    os.rm("$(curdir)/execs", { async = true })
    -- remove this line to test if the configuration file changes
    os.cp("$(curdir)/rpgpp.ini", "$(builddir)/$(plat)/$(arch)/$(mode)/", { copy_if_different = true })
end)
