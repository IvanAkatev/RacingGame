#pragma once
#include "core/track/ITrack.h"

class StraightTrack final : public ITrack {
public:
    StraightTrack(const Vec2& start, const Vec2& end);

    float length() const override { return length_; }
    float wrapDistance(float s) const override;
    TrackSample sample(float s) const override;

private:
    Vec2 start_;
    Vec2 end_;
    Vec2 dir_;
    float length_ = 0.f;
};
