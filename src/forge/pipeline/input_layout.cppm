module;
#include <cassert>
#include <cstdint>


export module forge.pipeline:input_layout;

import std;
import forge.core.log;
import forge.core.math;

export namespace forge::pipeline {
enum class Format { Float2 = 8, Float3 = 12, Float4 = 16 };

enum class Semantic { Position,
    Normal, Color, UV, Tangent, Bitangent,SV_Position,Custom,SV_ID,SV_Lerp ,MaxCount };

// 输入元素描述，当前默认所有数据vec4存储对齐
const uint32_t DefaultElementSize = 16;
struct InputElement {
    Format format;
    Semantic semantic;
    uint32_t offset;
};

class RawLayout {
    std::vector<InputElement> m_elements;

  public:
    RawLayout(std::vector<InputElement> elements) : m_elements(elements) {
        int offset = 0;
        for (auto &element : m_elements) {
            element.offset = offset;
            offset += DefaultElementSize; // vec4 对齐
        }
    }

    std::vector<InputElement> &elements() { return m_elements; }

    uint32_t stride() const { return m_elements.size() * DefaultElementSize; }

    int32_t offset(Semantic semantic) const {
        for (auto &element : m_elements) {
            if (element.semantic == semantic) {
                return element.offset;
            }
        }
        return -1;
    }
};

struct AttributeView {
    std::span<const std::byte> attributes;
    const RawLayout &layout;

    // 读取属性数据 auto x = AV.view<Vector3>(Semantic::Position);
    template <typename T> T view(Semantic semantic) {
        uint32_t offset = layout.offset(semantic);
        if (offset == -1) {
            forge::log_error("RawLayout: semantic not found");
        }

        return *reinterpret_cast<T *>(attributes.data() + offset);
    }
};

// 聚合数据，将所有的属性数据聚合在一起，方便数据访问
class PipelineData {
  public:
    RawLayout input_layout;
    std::span<std::byte> data_view;
    std::vector<std::byte> raw_data;
    PipelineData(const RawLayout &layout, std::vector<std::byte> &&data) : input_layout(layout), raw_data(std::move(data)) {
        // 必须在 raw_data 移动完成后再创建 span，否则 span 指向的是旧的/空的内存
        data_view = std::span<std::byte>(raw_data);
    }

    uint32_t count() {
        return data_view.size() / input_layout.stride();
    }

    AttributeView fetch(uint32_t index) const {
        uint32_t stride = input_layout.stride();
        uint32_t offset = index * stride;

        // 增加一个简单的安全断言
        assert(offset + stride <= data_view.size());

        auto slice = data_view.subspan(offset, input_layout.stride());

        return AttributeView{.attributes = slice, .layout = input_layout};
    }

    std::span<std::byte> data() { return data_view; }
    
};

template <typename T> 
    class PipelineDataBuilder {
    RawLayout m_layout;
    std::function<const void *(const T &, Semantic)> getter;
    std::span<const T> attributes;

  public:
    PipelineDataBuilder(const RawLayout &layout) : m_layout(layout) {}

    void put(std::vector<T> &attr) { attributes = attr; }

    void set_getter(std::function<const void *(const T &, Semantic)> getter) { this->getter = getter; }

    PipelineData build() {
        std::vector<std::byte> ret_data;
        ret_data.reserve(attributes.size() * m_layout.stride());

        for (auto &attribute : attributes) {
            for (auto &element : m_layout.elements()) {
                const size_t attribute_size = (size_t)element.format;
                const void *attr_ptr = getter(attribute, element.semantic);
                const std::byte *byte_ptr = reinterpret_cast<const std::byte *>(attr_ptr);

                ret_data.insert(ret_data.end(), byte_ptr, byte_ptr + attribute_size);

                // insert padding if needed
                size_t padding_size = DefaultElementSize - attribute_size;
                assert(padding_size >= 0);
                for (size_t i = 0; i < padding_size; ++i) {
                    ret_data.push_back(std::byte{0});
                }
            }
        }
        return {m_layout, std::move(ret_data)};
    }
};
} // namespace forge::pipeline