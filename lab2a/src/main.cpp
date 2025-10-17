#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1200}), "Spiral Movement - SFML 3.0");

    window.setFramerateLimit(120);

    sf::CircleShape shape(30.f);
    shape.setFillColor(sf::Color::Green);

    // SFML 3.0: setOrigin принимает только Vector2f
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));

    // Начальная позиция в центре экрана
    shape.setPosition(sf::Vector2f(1920.f / 2, 1200.f / 2));

    float angle = 0.0f;     // Угол для спирали
    float radius = 0.0f;    // Радиус спирали
    const float rotationSpeed = 2.0f;   // Скорость вращения
    const float spiralSpeed = 0.5f;     // Скорость расширения спирали

    while (window.isOpen()) {
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event.has_value()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
            }
        }

        // Обновление параметров спирали
        angle += rotationSpeed;
        radius += spiralSpeed;

        // Вычисление позиции по спирали относительно центра экрана
        float centerX = 1920.f / 2;
        float centerY = 1200.f / 2;
        float x = centerX + radius * std::cos(angle * 3.14159f / 180.f);
        float y = centerY + radius * std::sin(angle * 3.14159f / 180.f);

        // SFML 3.0: setPosition тоже принимает только Vector2f
        shape.setPosition(sf::Vector2f(x, y));
        window.clear(sf::Color::Black);
        window.draw(shape);
        window.display();
    }

    return 0;
}