#include "core/GameModel.h"
#include "core/track/StraightTrack.h"
#include "core/track/CircleTrack.h"


GameModel::GameModel() {
    track_ = std::make_unique<CircleTrack>(Vec2{960.f, 490.f}, 400.f);
    // track_ = std::make_unique<StraightTrack>(Vec2{100.f, 320.f}, Vec2{1800.f, 320.f});
    vehicle_.setDistance(0.f);
}

void GameModel::update(float dt, const InputSnapshot& input) {
    TrackSample ts = track_->sample(vehicle_.distance());
    vehicle_.update(dt, input.throttle, ts);
    vehicle_.setDistance(track_->wrapDistance(vehicle_.distance()));
}

TrackSample GameModel::vehicleSample() const {
    return track_->sample(vehicle_.distance());
}
