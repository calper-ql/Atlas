#ifndef ATLAS_HPP
#define ATLAS_HPP

#include <cstdint>
#include <string>
#include <cmath>

//                               9223372036854775807
const int64_t __ATLAS_TEXEL_SIZE_I = 100000000000000;
const double __ATLAS_TEXEL_SIZE_D = (double)__ATLAS_TEXEL_SIZE_I;
const double __ATLAS_KM = 1000.0;

struct AtlasDim
{
    int64_t macroDist = 0; // In KM
    int64_t microDist = 0; // Represents a KM
};

struct AtlasPosition
{
    AtlasDim x;
    AtlasDim y;
    AtlasDim z;
};

struct AtlasVector
{
    double x = 0;
    double y = 0;
    double z = 0;
};

AtlasDim atlasDimRegulate(AtlasDim A)
{
    auto absMicro = abs(A.microDist);
    if (absMicro >= __ATLAS_TEXEL_SIZE_I)
    {
        auto microDividend = absMicro / __ATLAS_TEXEL_SIZE_I;

        auto macroDiff = microDividend;
        auto microDiff = absMicro - macroDiff * __ATLAS_TEXEL_SIZE_I;

        if (A.microDist < 0)
        {
            A.microDist = -microDiff;
            A.macroDist += -macroDiff;
        }
        else
        {
            A.microDist = microDiff;
            A.macroDist += macroDiff;
        }
    }
    return A;
}

AtlasDim atlasDimAdd(AtlasDim A, AtlasDim B)
{
    A.macroDist = (A.macroDist + B.macroDist);
    A.microDist = (A.microDist + B.microDist);
    A = atlasDimRegulate(A);
    return A;
}

AtlasDim atlasDimDist(AtlasDim A, AtlasDim B)
{
    A.macroDist -= B.macroDist;
    A.microDist -= B.microDist;
    A.macroDist = abs(A.macroDist);
    A.microDist = abs(A.microDist);
    return A;
}

AtlasPosition atlasPositionAdd(AtlasPosition A, AtlasPosition B)
{
    A.x = atlasDimAdd(A.x, B.x);
    A.y = atlasDimAdd(A.y, B.y);
    A.z = atlasDimAdd(A.z, B.z);
    return A;
}

std::string atlasDimToString(AtlasDim dim)
{
    return "|" + std::to_string(dim.macroDist) + ", " + std::to_string(dim.microDist) + "|";
}

double atlasDimToDouble(AtlasDim dim)
{
    return dim.macroDist * __ATLAS_KM + dim.microDist * (__ATLAS_KM / __ATLAS_TEXEL_SIZE_D);
}

AtlasDim atlasDoubleToDim(double value)
{
    AtlasDim dim;
    auto abs_val = abs(value);
    dim.macroDist = (int64_t)std::floor(abs_val / __ATLAS_KM);

    abs_val = abs_val / __ATLAS_KM;
    dim.microDist = (int64_t)((abs_val - std::floor(abs_val)) * __ATLAS_TEXEL_SIZE_D);

    if (value < 0.0)
    {
        dim.macroDist = -dim.macroDist;
        dim.microDist = -dim.microDist;
    }

    return dim;
}

AtlasVector atlasPositionToVector(AtlasPosition position)
{
    auto vec = AtlasVector();
    vec.x = atlasDimToDouble(position.x);
    vec.y = atlasDimToDouble(position.y);
    vec.z = atlasDimToDouble(position.z);
    return vec;
}

AtlasPosition atlasVectorToPosition(AtlasVector vector)
{
    auto pos = AtlasPosition();
    pos.x = atlasDoubleToDim(vector.x);
    pos.y = atlasDoubleToDim(vector.y);
    pos.z = atlasDoubleToDim(vector.z);
    return pos;
}

#endif