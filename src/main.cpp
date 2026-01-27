#include <iostream>

import forge.core.log;
import forge.platform.window;

int main(int argc, char *argv[]) {
    const int width = 800;
    const int height = 600;

    forge::platform::Window app("Forge Renderer", width, height);

    // 模拟我们的 FrameBuffer (RGBA8888)
    std::vector<uint32_t> frame_buffer(width * height, 0xFF000000); // 初始黑色


    int FrameIdx = 0;
    forge::log_info("Starting Frame{}", FrameIdx);
    while (!app.should_close()) {
        app.update_events();

        // 这里是后续放置渲染逻辑的地方
        // 简单画一个随时间变化的背景色测试
        static uint8_t color = 0;
        for (auto &pixel : frame_buffer)
            pixel = 0xFF000000 | (color << 8);
        color++;

        app.update_frame(frame_buffer.data());
        forge::log_info("Current Frame{}", ++FrameIdx);
    }
    return 0;
}