#include <gtest/gtest.h>

#include "geometry.h"

TEST(math, Vector_ConstructVec2) {
    mini3d::Vec2 v2{1, 1};
    EXPECT_TRUE(v2 == mini3d::Vec2({1, 1}));
    EXPECT_TRUE(v2 != mini3d::Vec2({1, 0}));
    EXPECT_EQ(v2.get_x(), 1);
    EXPECT_EQ(v2.get_y(), 1);
}

TEST(math, Vector_ConstructVec4) {
    mini3d::Vector v4{1, 2, 3, 4.0};
    EXPECT_TRUE(v4 == mini3d::Vector({1, 2, 3, 4.0}));
    EXPECT_TRUE(v4 != mini3d::Vector({1, 2, 3, 0}));
    EXPECT_EQ(v4.get_x(), 1);
    EXPECT_EQ(v4.get_y(), 2);
    EXPECT_EQ(v4.get_z(), 3);
    EXPECT_EQ(v4.get_w(), 4);
}

TEST(math, Vector_Increments) {
    mini3d::Vector v4{1, 1, 1, 1};

    EXPECT_EQ(v4.IncX(), 2);
    EXPECT_TRUE(v4 == mini3d::Vector({2, 1, 1, 1}));

    EXPECT_EQ(v4.IncY(), 2);
    EXPECT_TRUE(v4 == mini3d::Vector({2, 2, 1, 1}));

    EXPECT_EQ(v4.IncZ(), 2);
    EXPECT_TRUE(v4 == mini3d::Vector({2, 2, 2, 1}));

    EXPECT_EQ(v4.IncW(), 2);
    EXPECT_TRUE(v4 == mini3d::Vector({2, 2, 2, 2}));
}

TEST(math, Vector_Decrements) {
    mini3d::Vector v4{2, 2, 2, 2};

    EXPECT_EQ(v4.DecX(), 1);
    EXPECT_TRUE(v4 == mini3d::Vector({1, 2, 2, 2}));

    EXPECT_EQ(v4.DecY(), 1);
    EXPECT_TRUE(v4 == mini3d::Vector({1, 1, 2, 2}));

    EXPECT_EQ(v4.DecZ(), 1);
    EXPECT_TRUE(v4 == mini3d::Vector({1, 1, 1, 2}));

    EXPECT_EQ(v4.DecW(), 1);
    EXPECT_TRUE(v4 == mini3d::Vector({1, 1, 1, 1}));
}

TEST(math, Matrix_Constructor) {
    mini3d::Matrix m{
        1.0f,  2.0f,  3.0f,  4.0f,   // 1
        5.0f,  6.0f,  7.0f,  8.0f,   // 2
        9.0f,  10.0f, 11.0f, 12.0f,  // 3
        13.0f, 14.0f, 15.0f, 16.0f   // 4
    };
    EXPECT_EQ(m.get_value(0, 0), 1.0f);
    EXPECT_EQ(m.get_value(0, 1), 2.0f);
    EXPECT_EQ(m.get_value(0, 2), 3.0f);
    EXPECT_EQ(m.get_value(0, 3), 4.0f);
    EXPECT_EQ(m.get_value(1, 0), 5.0f);
    EXPECT_EQ(m.get_value(1, 1), 6.0f);
    EXPECT_EQ(m.get_value(1, 2), 7.0f);
    EXPECT_EQ(m.get_value(1, 3), 8.0f);
    EXPECT_EQ(m.get_value(2, 0), 9.0f);
    EXPECT_EQ(m.get_value(2, 1), 10.0f);
    EXPECT_EQ(m.get_value(2, 2), 11.0f);
    EXPECT_EQ(m.get_value(2, 3), 12.0f);
    EXPECT_EQ(m.get_value(3, 0), 13.0f);
    EXPECT_EQ(m.get_value(3, 1), 14.0f);
    EXPECT_EQ(m.get_value(3, 2), 15.0f);
    EXPECT_EQ(m.get_value(3, 3), 16.0f);
}

TEST(math, Matrix_MutiplyIdentityMatrix) {
    mini3d::Matrix m{
        1.0f,  2.0f,  3.0f,  4.0f,   // 1
        5.0f,  6.0f,  7.0f,  8.0f,   // 2
        9.0f,  10.0f, 11.0f, 12.0f,  // 3
        13.0f, 14.0f, 15.0f, 16.0f   // 4
    };
    EXPECT_TRUE(m == m * mini3d::Matrix::IDENTITY());
}

TEST(math, Vector_MutiplyIdentityMatrix) {
    mini3d::Vector v4{1, 2, 3, 4};
    EXPECT_TRUE(v4 == mini3d::Matrix::IDENTITY() * v4);
}

TEST(math, Vector_AddMatrix) {
    mini3d::VectorImpl<float, 4> v1{1, 2, 3, 4}, v2{5, 6, 7, 8},
        v3{6, 8, 10, 12};
    EXPECT_TRUE(v3 == v1 + v2);
    EXPECT_TRUE(v1 == v3 - v2);
}

TEST(math, Matrix_MultiplyMatrix) {
    mini3d::MatrixImpl<int, 2, 2> m1{
        1, 2,  // 1
        3, 4   // 2
    };
    mini3d::MatrixImpl<int, 2, 2> m2{
        5, 6,  // 1
        7, 8   // 2
    };
    mini3d::MatrixImpl<int, 2, 2> m3{
        19, 22,  // 1
        43, 50   // 2
    };
    EXPECT_TRUE(m3 == m1 * m2);
    EXPECT_TRUE(m3 == m1 * m2 * (mini3d::MatrixImpl<int, 2, 2>::IDENTITY()));
}

TEST(math, Matrix_Transfer) {
    mini3d::Matrix m{
        1.0f,  2.0f,  3.0f,  4.0f,   // 1
        5.0f,  6.0f,  7.0f,  8.0f,   // 2
        9.0f,  10.0f, 11.0f, 12.0f,  // 3
        13.0f, 14.0f, 15.0f, 16.0f   // 4
    };
    mini3d::Matrix m2{
        1.0f,  2.0f,  3.0f,  1.0f,  // 1
        5.0f,  6.0f,  7.0f,  2.0f,  // 2
        9.0f,  10.0f, 11.0f, 3.0f,  // 3
        13.0f, 14.0f, 15.0f, 16.0f  // 4
    };
    m.Transfer({1, 2, 3, 1.0f});
    EXPECT_TRUE(m == m2);
}

TEST(math, Vector_Length) {
    mini3d::Vec2 v{1, 2};
    mini3d::F32 len = v.CalcLength();
    mini3d::F32 len2 = sqrt(1 + 4);
    EXPECT_EQ(len, len2);
}

TEST(math, Vector_Normalize) {
    mini3d::Vec2 v{1, 2};
    mini3d::F32 len = v.CalcLength();
    v.Normalize();
    mini3d::Vec2 v2{1 / len, 2 / len};
    EXPECT_TRUE(v.get_x() == v2.get_x());
}

TEST(math, Vector_Homogenize) {
    mini3d::Vector m{1, 2, 3, 2};
    m.Homogenize();
    EXPECT_EQ(m.get_x(), 1.0f / 2);
    EXPECT_EQ(m.get_y(), 2.0f / 2);
    EXPECT_EQ(m.get_z(), 3.0f / 2);
    EXPECT_EQ(m.get_w(), 1.0f);
}
