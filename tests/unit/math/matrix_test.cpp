#include <catch2/catch_test_macros.hpp> // 仅包含测试宏，不要包含 session
#include <catch2/matchers/catch_matchers_floating_point.hpp>

import forge.core.math;

TEST_CASE("Matrix4x4", "[math]")
{
    using namespace forge;
    
    Mat3 m1;
    Mat3 m2;

    SECTION("Identity Matrix") {
        m1 = Mat3::identity();

        REQUIRE(m1(0,0) == 1.0f);
        REQUIRE(m1(1,1) == 1.0f);
        REQUIRE(m1(2,2) == 1.0f);
        REQUIRE(m1(0,1) == 0.0f);
        REQUIRE(m1(0,2) == 0.0f);
        REQUIRE(m1(1,0) == 0.0f);
        REQUIRE(m1(1,2) == 0.0f);
        REQUIRE(m1(2,0) == 0.0f);
        REQUIRE(m1(2,1) == 0.0f);

    }

    



}