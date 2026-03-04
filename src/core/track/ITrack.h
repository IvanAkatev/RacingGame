#pragma once
#include "core/track/TrackSample.h"

class ITrack {
public:
    virtual ~ITrack() = default;

    virtual float length() const = 0;
    virtual float wrapDistance(float s) const = 0;
    virtual TrackSample sample(float s) const = 0;
};
