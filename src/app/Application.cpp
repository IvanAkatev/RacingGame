#include "app/Application.h"
#include <algorithm>
#include <optional>

int Application::run() {
    window_.setVerticalSyncEnabled(true);

    sf::Clock clock;
    float accumulator = 0.f;
    const float fixedDt = 1.f / 300.f;

    while (window_.isOpen()) {
        InputSnapshot input;
        handleInput(input);

        float frameDt = clock.restart().asSeconds();
        offtrackMessageLeft_ = std::max(0.f, offtrackMessageLeft_ - frameDt);
        accumulator += frameDt;

        while (accumulator >= fixedDt) {
            update(fixedDt, input);
            accumulator -= fixedDt;
        }

        render();
    }

    return 0;
}

void Application::handleInput(InputSnapshot& input) {
    while (const std::optional<sf::Event> event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window_.close();
        } else if (const auto* kp = event->getIf<sf::Event::KeyPressed>()) {
            if (kp->scancode == sf::Keyboard::Scancode::Escape) {
                window_.close();
            }
        }
    }

    input.throttle = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
}

void Application::update(float dt, const InputSnapshot& input) {
    model_.update(dt, input);
    if (model_.vehicle().offtrack()) {
        offtrackMessageLeft_ = 1.5f;
    }
}

void Application::render() {
    window_.clear();
    renderer_.draw(window_, model_, offtrackMessageLeft_ > 0.f);
    window_.display();
}
