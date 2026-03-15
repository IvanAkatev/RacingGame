#include "core/track/CircleTrack.h"
#include <cmath>

CircleTrack::CircleTrack(const Vec2& center, float radius)
    : center_(center), radius_(radius) {
    const float twoPi = 6.28318530718f;
    length_ = twoPi * radius_;
}

float CircleTrack::wrapDistance(float s) const {
    if (length_ <= 1e-6f) return 0.f;
    float m = std::fmod(s, length_);
    if (m < 0.f) m += length_;
    return m;
}

TrackSample CircleTrack::sample(float s) const {
    TrackSample ts;
    float ss = wrapDistance(s);

    const float twoPi = 6.28318530718f;
    float theta = twoPi * (ss / length_); // от 0 до 2π

    float c = std::cos(theta);
    float sn = std::sin(theta);

    ts.position = center_ + Vec2{ c * radius_, sn * radius_ };
    ts.tangent  = Vec2{ -sn, c };  // движение против часовой
    ts.curvature = 1.0f / radius_; // по желанию, если используется
    return ts;
}