module;
#include "SDL3/SDL.h" 

export module forge.platform.window;
import forge.core.log;
import std;

export namespace forge::platform {
class Window {
    SDL_Window *window_ = nullptr;
    
    SDL_Texture *texture_ = nullptr;
    int width_, height_;
    bool should_close_ = false;

  public:
    SDL_Renderer *renderer_ = nullptr;
    Window(std::string title, int width, int height) : width_(width), height_(height) {

        if (SDL_Init(SDL_INIT_VIDEO) == false) {
            forge::log_error("Failed to initialize SDL");
            // forge::log_error("Failed to initialize SDL: {}", SDL_GetError());
            return;
        }
        window_ = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
        if(!window_){
            forge::log_error("Failed to create window");
        }
        SDL_SetWindowPosition(window_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

        renderer_ = SDL_CreateRenderer(window_, nullptr);
        // 创建一个流式纹理，用于将 CPU 渲染的像素数据拷贝到 GPU 显示
        texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        if(!texture_){
            forge::log_error("Failed to create texture");
        }

    }

    ~Window() {
        SDL_DestroyTexture(texture_);
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

    bool should_close() const { return should_close_; }

    // 刷新屏幕缓冲
    void update_frame(const void *pixels) {
        SDL_UpdateTexture(texture_, nullptr, pixels, width_ * 4);
        SDL_RenderClear(renderer_);
        SDL_RenderTexture(renderer_, texture_, nullptr, nullptr);
        SDL_RenderPresent(renderer_);
    }
    void update_events() {
        SDL_Event event{};
        forge::log_debug("begin SDL event polling");
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                should_close_ = true;
                break;
                
            case SDL_EVENT_WINDOW_EXPOSED:  // 窗口显示
                break;
            }
        }
        forge::log_debug("SDL event polling");

    }
};
} // namespace forge::platform