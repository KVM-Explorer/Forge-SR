module;


export module forge.pipeline:vertex;
import : assembly;

namespace forge::pipeline {

struct VSoutAttachment {
    Vector4 sv_position;
    Vector3 positionNDC;
    Vector2 positionSS;
    float inW;
};

class StageBase {

  public:
    using ShaderProgam = std::function<void(AttributeView, AttributeView)>;

    StageBase(RawLayout in, RawLayout out);
public:
    void execute(std::span<std::byte> in,std::span<std::byte> out,int count) {
        for (int i=0;i<count;i++) {
            auto view1 = AttributeView{
                in.subspan(i *input_layout.stride(), input_layout.stride()),
                input_layout};
            auto view2 = AttributeView{
                out.subspan(i * output_layout.stride(), output_layout.stride()), 
                output_layout};
            program(view1, view2);
        }
    }

    RawLayout input_layout;
    RawLayout output_layout;
    ShaderProgam program;
};

export class VertexStage : public StageBase {
public:
    VertexStage(RawLayout in, RawLayout out);
    void bind(StageBase::ShaderProgam program);
    
    size_t customSize(){
        return output_layout.stride();
    }
};

class FragmentStage: public StageBase{
public:
    FragmentStage(RawLayout in, RawLayout out);
    void bind(StageBase::ShaderProgam program);
};






class TransformStage{
    public: 
    void execute(std::span<std::byte> in,std::span<std::byte> out)
    {
        
    }
};

}