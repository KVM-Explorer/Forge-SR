export module forge.core.math:constants;

import std;
export namespace forge{
    // 数学常量
    constexpr float PI = std::numbers::pi_v<float>;
    constexpr float PI_2 = std::numbers::pi_v<float> * 2.0f;
    constexpr float INV_PI = 1.0f / std::numbers::pi_v<float>;
    constexpr float INV_PI_2 = 1.0f / (std::numbers::pi_v<float> * 2.0f);

    constexpr float EPSILON = 1e-6f;
    constexpr float EPSILON_SQR = EPSILON * EPSILON;

    constexpr float INF = std::numeric_limits<float>::infinity();
    constexpr float NAN = std::numeric_limits<float>::quiet_NaN();
    constexpr float NAN_SQR = NAN * NAN;
}