
export module forge.pipeline.canvas;
import std;

export namespace forge::pipeline {
    using Color = int;

    class Canvas {
    public:
        Canvas(int width, int height) 
            : width_(width), height_(height), data_(width * height, 0xFF000000) {
            // 创建一个 2D 视图：[行, 列]
            // extents<类型, 维度1, 维度2>
            view_ = std::mdspan(data_.data(), height_, width_);
        }

        void set_pixel(int x, int y, Color color) {
            if (x >= 0 && x < width_ && y >= 0 && y < height_) {
                // C++23 的多维下标运算符 []
                view_[y, x] = color;
            }
        }

        const Color* raw_data() const { return data_.data(); }

    private:
        int width_, height_;
        std::vector<Color> data_;
        // mdspan 成员：动态维度
        std::mdspan<Color, std::dextents<size_t, 2>> view_;
    };
}