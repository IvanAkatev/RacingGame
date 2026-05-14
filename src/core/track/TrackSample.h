#pragma once
#include "math/Vec2.h"

struct TrackSample {
    Vec2 position;
    Vec2 tangent;
    Vec2 normal;
    float curvature = 0.f;
    float maxLatAccel = 1e9f;
    float halfWidth = 30.f;
    float centerlineGrip = 1.f;
    float offtrackGrip = 0.65f;
};
