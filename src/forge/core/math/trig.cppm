export module forge.core.math:trig;

import :constants;
export namespace forge{

    struct Radian {
        float value;
        Radian(float value) : value{value} {}

        float to_degree() const {
            return value * (180.0f / PI);
        }
        
    };

    struct Degree {
        float value;
        Degree(float value) : value{value} {}
        float to_radian() const {
            return value * (PI / 180.0f);
        }
    };

    inline float sin(Radian r) {
        return std::sin(r.value);
    }

    inline float cos(Radian r) {
        return std::cos(r.value);
    }

    inline float tan(Radian r) {
        return std::tan(r.value);
    }

    inline float asin(float x) {
        return std::asin(x);
    }

    inline float acos(float x) {
        return std::acos(x);
    }

    inline float atan(float x) {
        return std::atan(x);
    }






}