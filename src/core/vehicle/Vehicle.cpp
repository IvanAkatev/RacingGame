#include "core/vehicle/Vehicle.h"
#include <algorithm>

Vehicle::Vehicle() : p_(Params{}) {}
Vehicle::Vehicle(const Params& p) : p_(p) {}

void Vehicle::enterSpin() {
    state_ = State::Spinning;
    spinLeft_ = p_.spinDuration;
    v_ = 0.f;
}

void Vehicle::update(float dt, bool throttle, const TrackSample& ts) {
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

    float lat = v_ * v_ * ts.curvature;
    if (lat > ts.maxLatAccel) {
        enterSpin();
    }
}
