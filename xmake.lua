-- 设置项目信息
set_project("Forge")
set_version("0.1.0")

-- 强制使用 C++20/23 标准
set_languages("c++23")

-- 添加依赖模块
add_requires("libsdl3")

-- 编译规则：添加所有模块文件
target("forge_core")
    set_kind("static")
    -- 递归添加所有 .mpp 模块接口文件
    -- 重要跨模块引用，请使用 {public=true}，使得外部可访问当前内部模块
    add_files("src/forge/**.cppm",{public=true})  
    -- 如果有传统的 .cpp 实现文件也可以添加
    add_files("src/forge/**.cpp")
    add_packages("libsdl3",{public=true})

target("ForgeApp")
    set_kind("binary")
    add_deps("forge_core")
    add_files("src/main.cpp")

includes("tests/**.lua")