
import forge.pipeline;
import forge.core;
import forge.core.math;

int main() {
    // 1. 定义我们原始的顶点结构
    struct Vertex {
        forge::Vector3 pos;
        forge::Vector3 color;
    };

    // 2. 配置 Layout：我们要求 Position 和 UV 都对齐到 Vec4 存储
    forge::pipeline::InputLayout layout(
        {
            {forge::pipeline::Format::Float3, forge::pipeline::Semantic::Position}, {forge::pipeline::Format::Float2, forge::pipeline::Semantic::UV}});

    // 3. 准备原生数据
    std::vector<Vertex> triangle_vertices = {
        {{0.0f, 0.5f, 0.0f}, 
        {0.0f, 1.0f, 0.0f}}, 
        {{-0.5f, -0.5f, 0.0f}, 
        {0.0f, 0.0f, 1.0f}}, 
        {{0.5f, -0.5f, 0.0f},
         {1.0f, 1.0f, 1.0f}}};
    // 4. 使用 Builder 聚合数据
    forge::pipeline::PipelineDataBuilder<Vertex> builder(layout);
    builder.put(triangle_vertices);
    builder.set_getter([](const Vertex &v, forge::pipeline::Semantic s) -> const void * {
        using enum forge::pipeline::Semantic;
        if (s == Position)
            return &v.pos;
        if (s == Color)
            return &v.color;
        forge::log_error("Semantic not found in Vertex structure");
        return nullptr;
    });

    // 5. 生成最终的 PipelineData（这是零拷贝移动进去的）
    auto pipeline_data = builder.build();
}