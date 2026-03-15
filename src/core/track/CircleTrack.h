#pragma once
#include "core/track/ITrack.h"

class CircleTrack final : public ITrack {
public:
    CircleTrack(const Vec2& center, float radius);

    float length() const override { return length_; }
    float wrapDistance(float s) const override;
    TrackSample sample(float s) const override;

private:
    Vec2 center_;
    float radius_ = 0.f;
    float length_ = 0.f;
};