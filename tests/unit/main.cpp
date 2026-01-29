#include <catch2/catch_session.hpp>

int main(int argc, char* argv[]) {
    // 可以在这里初始化全局日志系统或图形上下文
    int result = Catch::Session().run(argc, argv);
    return result;
}