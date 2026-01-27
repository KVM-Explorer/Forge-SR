export module forge.core.log;

import std;
export namespace forge::core {

enum class LogLevel {
    Success,Info, Error, Debug
};

class Logger {
public:
    // 使用 std::format_string 确保编译期格式检查
    template <typename... Args>
    static void log(LogLevel level, std::format_string<Args...> fmt, Args&&... args) {
        auto now = std::chrono::system_clock::now();
        
        // 简易颜色方案
        const char* color = "";
        const char* level_str = "";

        switch (level) {
            case LogLevel::Info:     color = "\033[37m"; level_str = "INFO"; break;
            case LogLevel::Success:  color = "\033[32m"; level_str = "PASS"; break;
            case LogLevel::Debug:    color = "\033[33m"; level_str = "Debug"; break;
            case LogLevel::Error:    color = "\033[31m"; level_str = "FAIL"; break;
        }

        // C++23 std::print 直接输出，性能优于 std::cout
        auto local_now = std::chrono::zoned_time{ std::chrono::current_zone(), now };
        std::print("[{:%H:%M:%S}] {}{:<6}\033[0m: ", local_now, color, level_str);
        std::println(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void success(std::format_string<Args...> fmt, Args&&... args,
                      const std::source_location loc = std::source_location::current()) {
        log(LogLevel::Success, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void debug(std::format_string<Args...> fmt, Args&&... args,
                      const std::source_location loc = std::source_location::current()) {
        log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void info(std::format_string<Args...> fmt, Args&&... args,
                      const std::source_location loc = std::source_location::current()) {
        log(LogLevel::Info, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void error(std::format_string<Args...> fmt, Args&&... args, 
                     const std::source_location loc = std::source_location::current()) {
        log(LogLevel::Error, fmt, std::forward<Args>(args)...);
        // 使用灰底色打印错误发生的代码位置
        std::println("\t\033[90m>> Located at: {}:{} ({})\033[0m", 
                     loc.file_name(), loc.line(), loc.function_name());
    }
};

}

// 导出便捷调用接口
export namespace forge {
    template <typename... Args>
    void log_success(std::format_string<Args...> fmt, Args&&... args) {
        core::Logger::log(core::LogLevel::Success, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void log_debug(std::format_string<Args...> fmt, Args&&... args) {
        core::Logger::log(core::LogLevel::Debug, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void log_info(std::format_string<Args...> fmt, Args&&... args) {
        core::Logger::log(core::LogLevel::Info, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void log_error(std::format_string<Args...> fmt, Args&&... args) {
        core::Logger::error(fmt, std::forward<Args>(args)...);
    }

    
}