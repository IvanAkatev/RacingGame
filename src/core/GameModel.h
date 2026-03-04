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

private:
    std::unique_ptr<ITrack> track_;
    Vehicle vehicle_;
};
