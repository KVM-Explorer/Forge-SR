export module forge.pipeline;
export import :input_layout;
export import :assembly;
export import :vertex;
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
    uint32_t stride;

    void add(Semantic semantic, int size);
};

class IntermediateBuffer {
    VaryingLayout layout;
    std::vector<float> data;
public:
    IntermediateBuffer(VaryingLayout in,int count) : layout(in) 
    {
        data.reserve(count * in.stride);
    }
};

class Buffer22 {
    RawLayout layout;
    std::vector<std::byte> data;

public:
    Buffer22(RawLayout in, int count) : layout(in) 
    { data.reserve(count * in.stride()); }
  std::span<std::byte> data_view() {
        return std::span(data.begin(),data.end());
    }
};

class Pipeline {
    int width;
    int height;
    PipelineData data;
    VertexStage vertex;
    FragmentStage frag;
    public:
        void setVertex(std::function<int()> func);
        void setPixel(std::function<Vector4()> func);
        void Execute() {

            //std::vector<TrianglePrimitive> primitives = Assembly::generate(data);
            // Step1: vertex stage
            std::vector<VSoutAttachment> vsOutputs;
            VaryingLayout layout;
            layout.add(Semantic::SV_Position, 4);
            layout.add(Semantic::Custom, vertex_stage.customSize());

            

            Buffer22 buffer0(vertex.output_layout,data.count());
            
            for (int i = 0; i < data.count(); i++) {
                auto item = data.fetch(i);
                
                vertex.execute(data.data_view,buffer0.data_view(),data.count()); // SV_Position,attribution1,attr2,...,attrn

                

                // Step2: Culling
                // Step3: NDC空间变换
                // Step4: Viewport变换（生成像素坐标）
                
                vsOutputs.push_back(VSoutAttachment(item));
            }

            std::vector<VSoutAttachment> data2;
            for (int i=0;i<data.count();i++) {
                VSoutAttachment tmp;
                tmp.inW = 1;
                tmp.positionNDC = 
            }



            // Step5：图元装配
            // Step6：光栅化阶段
            // Step7: 像素着色阶段
            std::vector<RSout> rsOutputs; // {pixel_pos,VSOut}



            // Step3: pixel stage
            std::vector<PSout> psOutputs;
            Canvas canvas(width, height);
            for (auto pixel : rsOutputs) {
                PSout color, = func2(pixel);
                
            }

        }


    };

}
