#include "core/vehicle/Vehicle.h"
#include <algorithm>
#include <cmath>

Vehicle::Vehicle() : p_(Params{}) {}
Vehicle::Vehicle(const Params& p) : p_(p) {}

void Vehicle::enterSpin() {
    state_ = State::Spinning;
    spinLeft_ = p_.spinDuration;
    v_ *= std::clamp(p_.offtrackSpeedPenalty, 0.f, 1.f);
}

void Vehicle::update(float dt, bool throttle, const TrackSample& ts) {
    offtrack_ = false;

    if (state_ == State::Spinning) {
        spinLeft_ -= dt;
        if (spinLeft_ <= 0.f) {
            spinLeft_ = 0.f;
            state_ = State::Normal;
        }
        return;
    }

    float a = throttle ? p_.engineAccel : -p_.coastDecel;
    v_ = std::clamp(v_ + a * dt, 0.f, p_.maxSpeed);

    s_ += v_ * dt;

    const float latAbs = v_ * v_ * std::abs(ts.curvature);
    const float excessLat = std::max(0.f, latAbs - ts.maxLatAccel);
    if (excessLat > 0.f) {
        const float outwardSign = (ts.curvature >= 0.f) ? -1.f : 1.f;
        const float driftVelocity = excessLat * p_.lateralDriftGain;
        lateralOffset_ += outwardSign * driftVelocity * dt;
    } else {
        const float recenterAlpha = std::clamp(p_.recenteringGain * dt, 0.f, 1.f);
        lateralOffset_ += (0.f - lateralOffset_) * recenterAlpha;
    }

    if (std::abs(lateralOffset_) > ts.halfWidth) {
        offtrack_ = true;
        lateralOffset_ = 0.f;
        enterSpin();
    }
}
