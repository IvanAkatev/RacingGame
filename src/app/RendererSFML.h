#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "core/GameModel.h"

class RendererSFML {
public:
    RendererSFML();
    void draw(sf::RenderTarget& target, const GameModel& model, bool showOfftrackMessage);

private:
    sf::VertexArray buildTrackRibbon(const ITrack& track, float width, int segments = 64) const;
    sf::VertexArray buildTrackRibbonSegment(
        const ITrack& track,
        float width,
        float startDistance,
        float segmentLength,
        int segments = 16
    ) const;
    void drawOfftrackMessage(sf::RenderTarget& target);
    void drawStartStraightMarker(sf::RenderTarget& target, const ITrack& track, float trackWidth);
    void drawLapHud(sf::RenderTarget& target, const GameModel& model);
    static std::string formatLapTime(float seconds);

    sf::Texture carTexture_;
    std::optional<sf::Sprite> carSprite_;
    bool carSpriteReady_ = false;
    sf::Font uiFont_;
    bool uiFontReady_ = false;
};
