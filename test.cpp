#include <iostream>
#include <cmath>
#include <gtest/gtest.h>
#include <random>

#include "atlas.hpp"

bool inRange(double value, double expected, double allowedRange)
{
    auto in_range = (expected - allowedRange) <= value && value <= (expected + allowedRange);
    if (!in_range)
    {
        std::cout << " Value: " << value << "!!! Expected: " << expected << "        - range: " << allowedRange << std::endl;
    }
    return in_range;
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(DimDistTest, MacroDistTest)
{
    AtlasDim d1;
    d1.macroDist = -2;
    d1.microDist = __ATLAS_TEXEL_SIZE_D / 2;

    AtlasDim d2;
    d2.macroDist = 5;
    d2.microDist = __ATLAS_TEXEL_SIZE_D / 2;

    auto d3 = atlasDimDist(d1, d2);
    ASSERT_EQ(7, d3.macroDist);
    ASSERT_EQ(0, d3.microDist);
}

TEST(DimDistTest, MicroDistTest1)
{
    AtlasDim d1;
    d1.macroDist = -2;
    d1.microDist = 10;

    AtlasDim d2;
    d2.macroDist = 5;
    d2.microDist = 20;

    auto d3 = atlasDimDist(d1, d2);
    ASSERT_EQ(7, d3.macroDist);
    ASSERT_EQ(10, d3.microDist);
}

TEST(DimDistTest, MicroDistTest2)
{
    AtlasDim d1;
    d1.macroDist = -2;
    d1.microDist = 20;

    AtlasDim d2;
    d2.macroDist = 5;
    d2.microDist = 20;

    auto d3 = atlasDimDist(d1, d2);
    ASSERT_EQ(7, d3.macroDist);
    ASSERT_EQ(0, d3.microDist);
}

TEST(DimDistTest, MicroDistTest3)
{
    AtlasDim d1;
    d1.macroDist = -2;
    d1.microDist = 30;

    AtlasDim d2;
    d2.macroDist = 5;
    d2.microDist = 20;

    auto d3 = atlasDimDist(d1, d2);
    ASSERT_EQ(7, d3.macroDist);
    ASSERT_EQ(10, d3.microDist);
}

TEST(AtlasDim, DimToDoubleAndBack)
{
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-1e4, 1e4);

    for (size_t i = 0; i < 1000; i++)
    {
        auto double_value = distribution(generator);
        auto dim_value = atlasDoubleToDim(double_value);
        ASSERT_TRUE(inRange(atlasDimToDouble(dim_value), double_value, 1e-1));
    }
}

TEST(AtlasDim, RandomWalk1)
{
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-1e5, 1e5);

    for (size_t i = 0; i < 10000; i++)
    {
        auto diff = distribution(generator);
        AtlasDim dim;
        double dpos = 0.0;
        for (size_t j = 0; j < 100; j++)
        {

            dim = atlasDimAdd(dim, atlasDoubleToDim(diff));
            dpos += diff;
        }
        ASSERT_TRUE(inRange(atlasDimToDouble(dim), dpos, 1e-6));
    }
}

TEST(AtlasDim, RandomWalk2)
{
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-1e6, 1e6);

    for (size_t i = 0; i < 10000; i++)
    {
        AtlasDim dim;
        double dpos = 0.0;
        for (size_t j = 0; j < 1000; j++)
        {
            auto diff = distribution(generator);
            dim = atlasDimAdd(dim, atlasDoubleToDim(diff));
            dpos += diff;
        }
        ASSERT_TRUE(inRange(atlasDimToDouble(dim), dpos, 1e-6));
    }
}

TEST(AtlasDim, ISS)
{

    AtlasDim dim;
    double dpos = 0.0;
    for (size_t j = 0; j < 3; j++)
    {
        auto diff = 7660.32655543;
        dpos += diff;
        dim = atlasDimAdd(dim, atlasDoubleToDim(diff));
    }

    ASSERT_EQ(dim.macroDist, 22);
    ASSERT_TRUE(inRange(atlasDimToDouble(dim), dpos, 1e-9));
}

TEST(AtlasDim, ISS_NEGATIVE)
{

    AtlasDim dim;
    double dpos = 0.0;
    for (size_t j = 0; j < 3; j++)
    {
        auto diff = -7660.32655543;
        dpos += diff;
        dim = atlasDimAdd(dim, atlasDoubleToDim(diff));
    }

    ASSERT_EQ(dim.macroDist, -22);
    ASSERT_TRUE(inRange(atlasDimToDouble(dim), dpos, 1e-9));
}
