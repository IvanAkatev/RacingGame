#include "core/GameModel.h"
#include "core/track/StraightTrack.h"
#include "core/track/CircleTrack.h"


GameModel::GameModel() {
    track_ = std::make_unique<CircleTrack>(Vec2{960.f, 490.f}, 400.f);
    // track_ = std::make_unique<StraightTrack>(Vec2{100.f, 320.f}, Vec2{1800.f, 320.f});
    vehicle_.setDistance(0.f);
}

void GameModel::update(float dt, const InputSnapshot& input) {
    const float previousDistance = track_->wrapDistance(vehicle_.distance());
    currentLapTime_ += dt;

    TrackSample ts = track_->sample(vehicle_.distance());
    vehicle_.update(dt, input.throttle, ts);
    const float wrappedDistance = track_->wrapDistance(vehicle_.distance());
    vehicle_.setDistance(wrappedDistance);

    // Пересечение стартовой прямой (s = 0) фиксируем при переходе через длину трека.
    if (wrappedDistance < previousDistance && vehicle_.speed() > 1.f) {
        lastLapTime_ = currentLapTime_;
        hasLastLapTime_ = true;
        if (!hasBestLapTime_ || lastLapTime_ < bestLapTime_) {
            bestLapTime_ = lastLapTime_;
            hasBestLapTime_ = true;
        }
        currentLapTime_ = 0.f;
    }
}

TrackSample GameModel::vehicleSample() const {
    TrackSample ts = track_->sample(vehicle_.distance());
    ts.position = ts.position + ts.normal * vehicle_.lateralOffset();
    return ts;
}
