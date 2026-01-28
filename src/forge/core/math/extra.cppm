export module forge.core.math:extra;

import :vector;
export namespace forge { 

    // 重心插值
    Vector3 barycentric(const Vector3& v0, const Vector3& v1, const Vector3& v2, float u, float v) {
        return (1 - u - v) * v0 + u * v1 + v * v2;
    }

    // 线性插值
    float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    Vector3 lerp(const Vector3& a, const Vector3& b, float t) {
        return a + t * (b - a);
    }


    // 反射
    Vector3 reflect(const Vector3& i, const Vector3& n) {
        return i - 2.0f * dot(i, n) * n;
    }    
}