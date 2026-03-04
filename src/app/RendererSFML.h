#pragma once
#include <SFML/Graphics.hpp>
#include "core/GameModel.h"

class RendererSFML {
public:
    void draw(sf::RenderTarget& target, const GameModel& model);

private:
    sf::VertexArray buildTrackLine(const ITrack& track, int segments = 64) const;
};
