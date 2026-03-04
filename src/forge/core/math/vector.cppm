export module forge.core.math:vector;

import std;
export namespace forge {
    class Vector2 {
    public:
        float x;
        float y;

        Vector2(float x, float y) : x(x), y(y) {}
        Vector2() : x(0), y(0) {}
    };

    class Vector3 {
    public:
        float x;
        float y;
        float z;

        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        Vector3() : x(0), y(0), z(0){}
    };

    class Vector4 {
    public:
        float x;
        float y;
        float z;
        float w;

        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        Vector4() : x(0), y(0), z(0), w(0) {};
        //随机访问 xyzw 分量 行如xyz zxy wxyz
    };
}

export namespace forge{

    // Vector2 运算符重载
    auto operator +(const forge::Vector2& lhs, const forge::Vector2& rhs)
    {
        return forge::Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
    }
    auto operator -(const forge::Vector2& lhs, const forge::Vector2& rhs)
    {
        return forge::Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
    }
    auto operator *(const forge::Vector2& lhs, const forge::Vector2& rhs)
    {
        return forge::Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
    }
    auto operator *(const forge::Vector2& lhs, float rhs)
    {
        return forge::Vector2(lhs.x * rhs, lhs.y * rhs);
    }
    auto operator *(float lhs, const forge::Vector2& rhs)
    {
        return forge::Vector2(lhs * rhs.x, lhs * rhs.y);
    }
    float dot(const forge::Vector2& lhs, const forge::Vector2& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    // Vector3 运算符重载
    auto operator +(const forge::Vector3& lhs, const forge::Vector3& rhs)
    {
        return forge::Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }
    auto operator -(const forge::Vector3& lhs, const forge::Vector3& rhs)
    {
        return forge::Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
    }
    auto operator *(const forge::Vector3& lhs, const forge::Vector3& rhs)
    {
        return forge::Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
    }
    auto operator *(const forge::Vector3& lhs, float rhs)
    {
        return forge::Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
    }
    auto operator *(float lhs, const forge::Vector3& rhs)
    {
        return forge::Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
    }

    float dot(const forge::Vector3& lhs, const forge::Vector3& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }

    forge::Vector3 cross(const forge::Vector3& lhs, const forge::Vector3& rhs)
    {
        return forge::Vector3(lhs.y * rhs.z - lhs.z * rhs.y,
                                          lhs.z * rhs.x - lhs.x * rhs.z,
                                          lhs.x * rhs.y - lhs.y * rhs.x);
    }

    float length(const forge::Vector3& vec)
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }

    forge::Vector3 normalize(const forge::Vector3& vec)
    {
        float len = length(vec);
        return forge::Vector3(vec.x / len, vec.y / len, vec.z / len);
    }

    // Vector4 运算符重载
    auto operator +(const forge::Vector4& lhs, const forge::Vector4& rhs)
    {
        return forge::Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
    }
    auto operator -(const forge::Vector4& lhs, const forge::Vector4& rhs)
    {
        return forge::Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
    }
    auto operator *(const forge::Vector4& lhs, const forge::Vector4& rhs)
    {
        return forge::Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
    }
    auto operator *(const forge::Vector4& lhs, float rhs)
    {
        return forge::Vector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
    }
    auto operator *(float lhs, const forge::Vector4& rhs)
    {
        return forge::Vector4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
    }
    float dot(const forge::Vector4& lhs, const forge::Vector4& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
    }

    float length(const forge::Vector4& vec)
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
    }

    forge::Vector4 normalize(const forge::Vector4& vec)
    {
        float len = length(vec);
        return forge::Vector4(vec.x / len, vec.y / len, vec.z / len, vec.w / len);
    }



    // forge::Vector4 cross(const forge::Vector4& lhs, const forge::Vector4& rhs)
    // {
    //     return forge::Vector4(lhs.y * rhs.z - lhs.z * rhs.y,
    //                                       lhs.z * rhs.x - lhs.x * rhs.z,
    //                                       lhs.x * rhs.y - lhs.y * rhs.x,

    //     );
    // }



}
