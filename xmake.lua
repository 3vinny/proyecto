-- =============================================================================
-- CONFIGURACIÓN GLOBAL DEL PROYECTO
-- GRASIAS PANXITO :V
-- =============================================================================
set_project("fac")
set_languages("c11")

add_rules("mode.debug", "mode.release")

-- 1. SCOPE GLOBAL: 
if is_plat("linux") then
    add_requires("sdl2",       {system = true, configs = {sysconfig = {pc = "sdl2"}}})
    add_requires("sdl2_ttf",   {system = true, configs = {sysconfig = {pc = "sdl2_ttf"}}})
    add_requires("sdl2_image", {system = true, configs = {sysconfig = {pc = "sdl2_image"}}})
    add_requires("sdl2_mixer", {system = true, configs = {sysconfig = {pc = "sdl2_mixer"}}})
end

-- 2. SCOPE DEL TARGET:
target("game")
    set_kind("binary")
    add_includedirs("include")
    
    add_files("src/*.c")

    if is_plat("linux") then
        -- Adentro del target SOLO se enlazan con add_packages
        add_packages("sdl2", "sdl2_ttf", "sdl2_image", "sdl2_mixer")
        set_targetdir("linux")
    end

    if is_plat("linux", "freebsd", "bsd") then
        add_cflags("-Wall", "-Wextra", {tools = "clang"})

        if is_mode("debug") then
            add_cflags("-g", "-O0", {tools = "clang"})
            
            add_cflags("-fsanitize=address,undefined", {tools = "clang"})
            add_ldflags("-fsanitize=address,undefined", {tools = "clang"})
            
            set_policy("build.sanitizer.address", true)
            set_policy("build.sanitizer.undefined", true)
        end
    end

after_config(function ()
    import("core.project.project")
    project.export("compile_commands")
end)    
