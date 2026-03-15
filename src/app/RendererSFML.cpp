#include "app/RendererSFML.h"

static sf::Vector2f toSf(const Vec2& v) { return {v.x, v.y}; }

sf::VertexArray RendererSFML::buildTrackLine(const ITrack& track, int segments) const {
    sf::VertexArray va(sf::PrimitiveType::LineStrip, static_cast<std::size_t>(segments + 1));

    float L = track.length();
    for (int i = 0; i <= segments; ++i) {
        float t = (segments == 0) ? 0.f : static_cast<float>(i) / static_cast<float>(segments);
        float s = L * t;
        auto ts = track.sample(s);
        va[static_cast<std::size_t>(i)].position = toSf(ts.position);
    }
    return va;
}

void RendererSFML::draw(sf::RenderTarget& target, const GameModel& model) {
    auto line = buildTrackLine(model.track(), 128);
    target.draw(line);

    auto ts = model.vehicleSample();
    sf::CircleShape dot;
    dot.setRadius(8.f);
    dot.setOrigin(sf::Vector2f{8.f, 8.f});
    dot.setPosition(toSf(ts.position));

    dot.setFillColor(sf::Color(0, 66, 37));
    target.draw(dot);
}
