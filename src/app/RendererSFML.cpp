#include "app/RendererSFML.h"
#include <cmath>

static sf::Vector2f toSf(const Vec2& v) { return {v.x, v.y}; }

static float degFromTangent(const Vec2& t) {
    return std::atan2(t.y, t.x) * 180.f / 3.14159265f;
}

/// Перпендикуляр к касательной (левая сторона относительно направления движения).
static Vec2 perpLeft(const Vec2& tangent) {
    Vec2 n{-tangent.y, tangent.x};
    return n.normalized();
}

sf::VertexArray RendererSFML::buildTrackRibbon(const ITrack& track, float width, int segments) const {
    const float half = width * 0.5f;
    const std::size_t vertexCount = static_cast<std::size_t>(2 * (segments + 1));
    sf::VertexArray va(sf::PrimitiveType::TriangleStrip, vertexCount);

    const float L = track.length();
    const sf::Color asphalt(55, 55, 60);

    for (int i = 0; i <= segments; ++i) {
        float t = (segments == 0) ? 0.f : static_cast<float>(i) / static_cast<float>(segments);
        float s = L * t;
        auto ts = track.sample(s);
        Vec2 n = perpLeft(ts.tangent);
        Vec2 left = ts.position + n * half;
        Vec2 right = ts.position - n * half;

        std::size_t idx = static_cast<std::size_t>(2 * i);
        va[idx].position = toSf(left);
        va[idx].color = asphalt;
        va[idx + 1].position = toSf(right);
        va[idx + 1].color = asphalt;
    }
    return va;
}

void RendererSFML::draw(sf::RenderTarget& target, const GameModel& model) {
    constexpr float kTrackWidth = 60.f;
    auto ribbon = buildTrackRibbon(model.track(), kTrackWidth, 128);
    target.draw(ribbon);

    auto ts = model.vehicleSample();
    // sf::CircleShape dot;
    // dot.setRadius(8.f);
    // dot.setOrigin(sf::Vector2f{8.f, 8.f});
    // dot.setPosition(toSf(ts.position));
    // dot.setFillColor(sf::Color(250, 0, 0));
    // dot.setFillColor(sf::Color(0, 66, 37));
    // target.draw(dot);


    // Длина вдоль локальной +X — тогда угол = atan2(касательная) без сдвига на 90°.
    sf::RectangleShape rectangle(sf::Vector2f(20.f, 8.f));
    rectangle.setOrigin(sf::Vector2f{10.f, 4.f});
    rectangle.setPosition(toSf(ts.position));
    rectangle.setRotation(sf::degrees(degFromTangent(ts.tangent)));
    rectangle.setFillColor(sf::Color(0xEE71B5ff)); // Цвет заливки
    // rectangle.setFillColor(sf::Color(128, 255, 255)); // Цвет заливки
    rectangle.setOutlineThickness(1.f);     // Толщина обводки
    rectangle.setOutlineColor(sf::Color(0x00A1EAff)); // Цвет обводки
    target.draw(rectangle);

}
