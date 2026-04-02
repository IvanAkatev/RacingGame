#pragma once
#include "core/track/TrackSample.h"

class Vehicle {
public:
    struct Params {
        float engineAccel = 750.f;
        float coastDecel  = 250.f;
        float maxSpeed    = 800.f;
        float spinDuration = 0.7f;
    };

    enum class State { Normal, Spinning };

    Vehicle();
    explicit Vehicle(const Params& p);

    void setDistance(float s) { s_ = s; }
    float distance() const { return s_; }
    float speed() const { return v_; }
    State state() const { return state_; }

    void update(float dt, bool throttle, const TrackSample& ts);

private:
    void enterSpin();

    Params p_;
    float s_ = 0.f;
    float v_ = 0.f;

    State state_ = State::Normal;
    float spinLeft_ = 0.f;
};
