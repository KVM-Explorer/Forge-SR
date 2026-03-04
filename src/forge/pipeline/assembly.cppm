module;

export module forge.pipeline:assembly;
import :input_layout;

namespace forge::pipeline {


using TrianglePrimitive = std::array<AttributeView,3>;

class Assembly {
public:
    static std::vector<TrianglePrimitive> generate(PipelineData &input) { 
        std::vector<TrianglePrimitive> ret;
        
        int count = input.count();
        for (int i=0;i<count;i+=3) {
            ret.push_back({
                input.fetch(i),
                input.fetch(i + 1),
                input.fetch(i + 2)
            });
        }
        return std::move(ret);
    }
};

}
