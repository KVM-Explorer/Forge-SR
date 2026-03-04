
export module forge.pipeline;
export import :input_layout;
export import :assembly;
export import :vertex;
export import :rasterizer;
import forge.pipeline.canvas;


namespace forge::pipeline {

struct VSOut;
struct VSIn;
struct PSIn;
struct PSout;
struct RSout;

struct VaryingElement {
    Semantic semantic;
    uint32_t size;
    uint32_t offset;
};

struct VaryingLayout {
    std::vector<VaryingLayout> elements;

    size_t stride() { return stride_; }

    void add(Semantic semantic, int size);

  private:
    uint32_t stride_;
};

// 固定大小容器
class StaticBuffer {};

// 动态增长型容器
class StreamBuffer {
    VaryingLayout layout;
    std::vector<std::byte> data;

  public:
    StreamBuffer(VaryingLayout in, int count) : layout(in) { data.reserve(count * in.stride()); }
    StreamBuffer(VaryingLayout in) : layout(in) {}

    std::span<std::byte> data_view(int i) {
        int stride = layout.stride();
        int pos = stride * i;
        return std::span(data.begin() + pos, data.begin() + pos + stride);
    }

    std::span<std::byte> alloc(){
        size_t old_count = data.size();
        data.resize(old_count + layout.stride());
        // return
    }

    size_t count() const {
        return data.size();
    }
};

class DynamicView {
    VaryingLayout layout;
    DynamicView(VaryingLayout in) : layout(in) {}

    template <typename T>
    T view(Semantic semantic,std::span<std::byte> data) {
        uint32_t offset = layout.offset(semantic);
        if (offset == -1) {
            forge::log_error("RawLayout: semantic not found");
        }

        return *reinterpret_cast<T *>(data + offset);
    }
};

class Pipeline {
    int width;
    int height;
    PipelineData data;
    VertexStage vertex;
    FragmentStage frag;
    TransformStage transformer;
    Canvas renderTarget;
    Rasterizer raster;
  public:
    void setRT(Canvas target);
    void setVertex(std::function<int()> func);
    void setPixel(std::function<Vector4()> func);
    void Execute() {

        // std::vector<TrianglePrimitive> primitives = Assembly::generate(data);
        //  Step1: vertex stage
        int vertex_count = data.count();

        VaryingLayout geometry_layout;
        geometry_layout.add(Semantic::SV_Position, 4);
        StreamBuffer geometry_buffer(geometry_layout);

        // 遍历顶点ID
        for (int i = 0; i < vertex_count; i++) {
            auto item = data.fetch(i);
            auto buffer = geometry_buffer.data_view(i);
            vertex.execute(data.data_view, buffer); // SV_Position,attribution1,attr2,...,attrn
        }

        // Step2: Culling
        // Step3: NDC空间变换
        // Step4: Viewport变换（生成像素坐标）
        VaryingLayout transform_layout;
        transform_layout.add(Semantic::SV_Position, 4); // NDC Position(/w)
        StreamBuffer transform_buffer(transform_layout);

        for (int i = 0; i < vertex_count; i++) {
            auto in_buffer = geometry_buffer.data_view(i);
            auto out_buffer = transform_buffer.data_view(i);
            // buffer[i]/w -> transform_buffer
            transformer.execute(in_buffer, out_buffer);
        }

        // Step5：图元装配
        // Step6：光栅化阶段
        VaryingLayout raster_layout;
        raster_layout.add(Semantic::SV_Position, 4); // World Position
        raster_layout.add(Semantic::SV_ID);          // Triangle ID
        raster_layout.add(Semantic::SV_Lerp, 4 * 3);
        StreamBuffer raster_buffer(raster_layout);

        for (int i = 0; i < vertex_count; i += 3) {
            auto triangle_id = i / 3;
            auto v0 = data.fetch(i);
            auto v1 = data.fetch(i + 1);
            auto v2 = data.fetch(i + 2);
            raster.execute({v0, v1, v2}, raster_buffer);
        }

        // Step7: 像素着色阶段
        int pixel_count = raster_buffer.count();
        for (int i = 0; i < pixel_count; i++) {
            auto raw = raster_buffer.data_view(i);
            // Fetch Vertex
            int sx, sy;
            float depth;

            // Lerp Attribution

            // Pixel Compute
            // frag.execute()
            renderTarget.set_pixel(sx, sy, {}, depth);
        }
    }
};

} // namespace forge::pipeline
