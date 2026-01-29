#include <catch2/catch_test_macros.hpp> // 仅包含测试宏，不要包含 session
#include <catch2/matchers/catch_matchers_floating_point.hpp>

import forge.core.math;

TEST_CASE("Vector", "[math]") {
    using namespace forge;
    Vector2 v1{1.0f, 2.0f};
    Vector2 v2{3.0f, 4.0f};

    SECTION("Addition") {
        Vector2 result = v1 + v2;
        // 如果 Vector2 内部是 float，建议这样比：
        CHECK_THAT(result.x, Catch::Matchers::WithinAbs(4.0f, 0.0001f));
        CHECK_THAT(result.y, Catch::Matchers::WithinAbs(6.0f, 0.0001f));
    }

    SECTION("Dot Product") {
        float d = dot(v1, v2);
        REQUIRE_THAT(d, Catch::Matchers::WithinAbs(11.0f, 0.0001f));
    }

    SECTION("Cross Product") {
        Vector3 v3{1.0f, 2.0f, 3.0f};
        Vector3 v4{4.0f, 5.0f, 6.0f};
        Vector3 result = cross(v3, v4);
        REQUIRE_THAT(result.x, Catch::Matchers::WithinAbs(-3.0f, 0.0001f));
        REQUIRE_THAT(result.y, Catch::Matchers::WithinAbs(6.0f, 0.0001f));
        REQUIRE_THAT(result.z, Catch::Matchers::WithinAbs(-3.0f, 0.0001f));
    }

    SECTION("Scalar Multiplication") {
        Vector4 v5{1.0f, 2.0f, 3.0f, 4.0f};
        Vector4 v6{5.0f, 6.0f, 7.0f, 8.0f};
        Vector4 result = v5 * 2.0f;
        REQUIRE_THAT(result.x, Catch::Matchers::WithinAbs(2.0f, 0.0001f));
        REQUIRE_THAT(result.y, Catch::Matchers::WithinAbs(4.0f, 0.0001f));
        REQUIRE_THAT(result.z, Catch::Matchers::WithinAbs(6.0f, 0.0001f));
        REQUIRE_THAT(result.w, Catch::Matchers::WithinAbs(8.0f, 0.0001f));
    }
}