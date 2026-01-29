add_requires("catch2")

-- add unit tests

target("unit_tests")
    set_kind("binary")
    add_files("unit/main.cpp")
    add_files("unit/**/*.cpp")
    
    -- 核心：让测试程序能够 import 你的模块
    add_deps("forge_core") 
    
    -- 引入 Catch2
    add_packages("catch2")
    
    -- 运行测试程序
    after_build(function (target)
        os.execv(target:targetfile())
    end)
