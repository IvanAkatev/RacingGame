#pragma once
#include <memory>
#include "core/InputSnapshot.h"
#include "core/track/ITrack.h"
#include "core/vehicle/Vehicle.h"

class GameModel {
public:
    GameModel();

    void update(float dt, const InputSnapshot& input);

    const ITrack& track() const { return *track_; }
    const Vehicle& vehicle() const { return vehicle_; }
    TrackSample vehicleSample() const;
    float currentLapTime() const { return currentLapTime_; }
    bool hasLastLapTime() const { return hasLastLapTime_; }
    float lastLapTime() const { return lastLapTime_; }
    bool hasBestLapTime() const { return hasBestLapTime_; }
    float bestLapTime() const { return bestLapTime_; }

private:
    std::unique_ptr<ITrack> track_;
    Vehicle vehicle_;
    float currentLapTime_ = 0.f;
    float lastLapTime_ = 0.f;
    float bestLapTime_ = 0.f;
    bool hasLastLapTime_ = false;
    bool hasBestLapTime_ = false;
};
