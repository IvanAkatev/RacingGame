#include "core/track/StraightTrack.h"
#include <cmath>

StraightTrack::StraightTrack(const Vec2& start, const Vec2& end)
    : start_(start), end_(end) {
    Vec2 d = end_ - start_;
    length_ = d.length();
    dir_ = (length_ > 1e-6f) ? d.normalized() : Vec2{1.f, 0.f};
}

float StraightTrack::wrapDistance(float s) const {
    if (length_ <= 1e-6f) return 0.f;
    float m = std::fmod(s, length_);
    if (m < 0.f) m += length_;
    return m;
}

TrackSample StraightTrack::sample(float s) const {
    TrackSample ts;
    float ss = wrapDistance(s);
    ts.position = start_ + dir_ * ss;
    ts.tangent = dir_;
    return ts;
}
