#pragma once
#include "math/Vec2.h"

struct TrackSample {
    Vec2 position;
    Vec2 tangent;
    float curvature = 0.f;
    float maxLatAccel = 1e9f;
};
