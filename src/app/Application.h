#pragma once
#include <SFML/Graphics.hpp>

#include "core/GameModel.h"
#include "app/RendererSFML.h"

class Application {
public:
    int run();

private:
    void handleInput(InputSnapshot& input);
    void update(float dt, const InputSnapshot& input);
    void render();

private:
    sf::RenderWindow window_{sf::VideoMode(sf::Vector2u{1024u, 640u}), "Racing Skeleton (SFML 3 + vcpkg)"};
    GameModel model_;
    RendererSFML renderer_;
};
