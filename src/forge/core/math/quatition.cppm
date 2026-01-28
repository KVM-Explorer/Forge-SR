export module forge.core.math:quaternion;

export namespace forge{
    class Quaternion {
    public:

        float w;
        float x;
        float y;
        float z;

        Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
    };

    // Quaternion 运算符重载
    auto operator +(const Quaternion& lhs, const Quaternion& rhs)
    {
        return Quaternion(lhs.w + rhs.w, lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
    }
}