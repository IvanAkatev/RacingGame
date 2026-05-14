#include "app/RendererSFML.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string_view>

static sf::Vector2f toSf(const Vec2& v) { return {v.x, v.y}; }

static float degFromTangent(const Vec2& t) {
    return std::atan2(t.y, t.x) * 180.f / 3.14159265f;
}

/// Перпендикуляр к касательной (левая сторона относительно направления движения).
static Vec2 perpLeft(const Vec2& tangent) {
    Vec2 n{-tangent.y, tangent.x};
    return n.normalized();
}

RendererSFML::RendererSFML() {
    // Use OS font as a lightweight HUD fallback.
    uiFontReady_ = uiFont_.openFromFile("arial.ttf");

    if (!carTexture_.loadFromFile("assets/sprites/Lotus.png")) {
        return;
    }

    carSprite_.emplace(carTexture_);
    auto size = carTexture_.getSize();
    carSprite_->setOrigin(sf::Vector2f(size.x * 0.5f, size.y * 0.5f));

    // Подгонка масштаба под прежний размер машины 20x8.
    constexpr float targetLen = 40.f;
    constexpr float targetWid = 30.f;
    carSprite_->setScale(sf::Vector2f(
        targetLen / static_cast<float>(size.x),
        targetWid / static_cast<float>(size.y)
    ));

    carSpriteReady_ = true;
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

sf::VertexArray RendererSFML::buildTrackRibbonSegment(
    const ITrack& track,
    float width,
    float startDistance,
    float segmentLength,
    int segments
) const {
    const float half = width * 0.5f;
    const std::size_t vertexCount = static_cast<std::size_t>(2 * (segments + 1));
    sf::VertexArray va(sf::PrimitiveType::TriangleStrip, vertexCount);
    const sf::Color startStraight(95, 95, 118);

    for (int i = 0; i <= segments; ++i) {
        float t = (segments == 0) ? 0.f : static_cast<float>(i) / static_cast<float>(segments);
        float s = startDistance + segmentLength * t;
        auto ts = track.sample(s);
        Vec2 n = perpLeft(ts.tangent);
        Vec2 left = ts.position + n * half;
        Vec2 right = ts.position - n * half;

        std::size_t idx = static_cast<std::size_t>(2 * i);
        va[idx].position = toSf(left);
        va[idx].color = startStraight;
        va[idx + 1].position = toSf(right);
        va[idx + 1].color = startStraight;
    }
    return va;
}

void RendererSFML::draw(sf::RenderTarget& target, const GameModel& model, bool showOfftrackMessage) {
    constexpr float kTrackWidth = 60.f;
    constexpr float kStartStraightLength = 140.f;
    auto ribbon = buildTrackRibbon(model.track(), kTrackWidth, 128);
    target.draw(ribbon);
    auto startStraight = buildTrackRibbonSegment(model.track(), kTrackWidth, 0.f, kStartStraightLength, 24);
    target.draw(startStraight);
    drawStartStraightMarker(target, model.track(), kTrackWidth);

    auto ts = model.vehicleSample();

    if (carSpriteReady_) {
        carSprite_->setPosition(toSf(ts.position));
        carSprite_->setRotation(sf::degrees(degFromTangent(ts.tangent)));
        target.draw(*carSprite_);
    } else {
        // Fallback, если текстура не загрузилась.
        sf::RectangleShape rectangle(sf::Vector2f(20.f, 8.f));
        rectangle.setOrigin(sf::Vector2f{10.f, 4.f});
        rectangle.setPosition(toSf(ts.position));
        rectangle.setRotation(sf::degrees(degFromTangent(ts.tangent)));
        rectangle.setFillColor(sf::Color(0xEE, 0x71, 0xB5));
        rectangle.setOutlineThickness(1.f);
        rectangle.setOutlineColor(sf::Color(0x00, 0xA1, 0xEA));
        target.draw(rectangle);
    }

    if (showOfftrackMessage) {
        drawOfftrackMessage(target);
    }
    drawLapHud(target, model);
}

void RendererSFML::drawOfftrackMessage(sf::RenderTarget& target) {
    if (!uiFontReady_) {
        return;
    }

    constexpr std::string_view kMessage = "OFF TRACK!";
    sf::Text text(uiFont_, kMessage.data(), 64);
    text.setFillColor(sf::Color(255, 240, 80));
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(3.f);

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x * 0.5f, bounds.position.y + bounds.size.y * 0.5f));

    sf::View view = target.getView();
    text.setPosition(view.getCenter());
    target.draw(text);
}

void RendererSFML::drawStartStraightMarker(sf::RenderTarget& target, const ITrack& track, float trackWidth) {
    auto startSample = track.sample(0.f);
    sf::RectangleShape line(sf::Vector2f(trackWidth + 4.f, 5.f));
    line.setOrigin(sf::Vector2f((trackWidth + 4.f) * 0.5f, 2.5f));
    line.setPosition(toSf(startSample.position));
    line.setRotation(sf::degrees(degFromTangent(startSample.tangent) + 90.f));
    line.setFillColor(sf::Color::White);
    target.draw(line);
}

void RendererSFML::drawLapHud(sf::RenderTarget& target, const GameModel& model) {
    if (!uiFontReady_) {
        return;
    }

    std::ostringstream lines;
    lines << "Current: " << formatLapTime(model.currentLapTime()) << "\n";
    lines << "Last: " << (model.hasLastLapTime() ? formatLapTime(model.lastLapTime()) : std::string("--:--.---")) << "\n";
    lines << "Best: " << (model.hasBestLapTime() ? formatLapTime(model.bestLapTime()) : std::string("--:--.---"));

    sf::Text hud(uiFont_, lines.str(), 26);
    hud.setFillColor(sf::Color(245, 245, 245));
    hud.setOutlineColor(sf::Color::Black);
    hud.setOutlineThickness(2.f);

    sf::FloatRect bounds = hud.getLocalBounds();
    sf::View view = target.getView();
    const sf::Vector2f viewSize = view.getSize();
    const sf::Vector2f viewCenter = view.getCenter();
    const sf::Vector2f topLeft = viewCenter - viewSize * 0.5f;
    constexpr float margin = 22.f;
    hud.setPosition(sf::Vector2f(
        topLeft.x + viewSize.x - bounds.size.x - bounds.position.x - margin,
        topLeft.y + margin - bounds.position.y
    ));
    target.draw(hud);
}

std::string RendererSFML::formatLapTime(float seconds) {
    const int totalMillis = std::max(0, static_cast<int>(seconds * 1000.f + 0.5f));
    const int minutes = totalMillis / 60000;
    const int sec = (totalMillis / 1000) % 60;
    const int millis = totalMillis % 1000;

    std::ostringstream out;
    out << std::setfill('0') << std::setw(2) << minutes
        << ":" << std::setw(2) << sec
        << "." << std::setw(3) << millis;
    return out.str();
}
