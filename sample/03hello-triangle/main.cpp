
import forge.pipeline;
import forge.core;
import forge.core.math;
import forge.pipeline.canvas;

import std;
int main() {

    int width = 360;
    int height = 360;

    // 1. 定义我们原始的顶点结构(vsinput/output,psinput)
    struct Vertex {
        forge::Vector3 pos;
        forge::Vector3 color;
    };

    // 2. 配置 Layout：我们要求 Position 和 UV 都对齐到 Vec4 存储
    forge::pipeline::RawLayout layout(
        {
            {forge::pipeline::Format::Float3, forge::pipeline::Semantic::Position}, 
            {forge::pipeline::Format::Float3, forge::pipeline::Semantic::Color}
        });

    // 3. 准备原生数据
    std::vector<Vertex> triangle_vertices = {
        {{0.0f, 0.5f * width, 0.0f}, 
        {0.0f, 1.0f , 0.0f}}, 
        {{-0.5f * width, -0.5f * width, 0.0f}, 
        {0.0f, 0.0f, 1.0f}}, 
        {{0.5f * width, -0.5f * width, 0.0f},
         {1.0f, 1.0f, 1.0f}}};
    int indices_count = triangle_vertices.size();

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

    // 6. 定义画布,并拉取数据进行处理
    forge::pipeline::Canvas canvas(width, height);

    // vertex
    for (int i = 0; i < indices_count; i+=3)
    {
        using enum forge::pipeline::Semantic;
        for (int j = 0; j < 3; j++)
        {
            auto item = pipeline_data.fetch(i+j);
            auto position = item.view<forge::Vector3>(Position);
            auto color = item.view<forge::Vector3>(Color);
        }
        
    }
    // 光栅化
    // pixel

}