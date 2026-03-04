module;

export module fogre.pipeline:data;
import std;


class Stream{

};

class StreamAllocator{
    std::byte buffer[1024*1024 *64]; // 64MB

    public:
    template<typename T>
    auto alloc(size_t count){
        return std::span<T>(buffer);
    }

    void reset()
    {

    }
};
