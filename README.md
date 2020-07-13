# Atlas

A position system for representing more accurate position.

## Why ?

Because float64 is not enough to accurately represent even our solar system.

## How ?

We got a struct like:

```cpp
struct AtlasDim
{
    int64_t macroDist = 0; // In KM
    int64_t microDist = 0; // Represents a KM
};
```

This struct can represent approximately 1800 light years to sub micro meter accuracy.

To use this struct atlas provides with the following functions:

```cpp
AtlasDim atlasDimAdd(AtlasDim A, AtlasDim B);
AtlasDim atlasDimSub(AtlasDim A, AtlasDim B);
AtlasDim atlasDimDist(AtlasDim A, AtlasDim B);
std::string atlasDimToString(AtlasDim dim);
double atlasDimToDouble(AtlasDim dim);
AtlasDim atlasDoubleToDim(double value);
```

Keep in mind that double should be used to represent change in this context. A direct conversion from AtlasDim to double is not possible.

Atlas also supplies the following structs for ease of use in 3 dimensions:

```cpp
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

AtlasPosition atlasPositionAdd(AtlasPosition A, AtlasPosition B);
AtlasVector atlasPositionToVector(AtlasPosition position);
AtlasPosition atlasVectorToPosition(AtlasVector vector);
```

### Integration to code

Take atlas.hpp and include it? if doesn't work create issue pls.

### But Can(Jon), how do I use this to render stuff?

Pass the Position struct to your shader. Once in the shader extract camera position from the position. Translate to vector in your shader (if you look at `atlasDimToDouble` its pretty easy to do).

```cpp
const int64_t __ATLAS_TEXEL_SIZE_I = 100000000000000;
const double __ATLAS_TEXEL_SIZE_D = (double)__ATLAS_TEXEL_SIZE_I;
const double __ATLAS_KM = 1000.0;

double atlasDimToDouble(AtlasDim dim)
{
    // You might want write these values as constants in your shader.
    return dim.macroDist * __ATLAS_KM + dim.microDist * (__ATLAS_KM / __ATLAS_TEXEL_SIZE_D);
}
```

### Missing features

I need to come up with a way to calculate distance in 3d. Some other math functions are missing as well.
