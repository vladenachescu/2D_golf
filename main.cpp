#include <SFML/Graphics.hpp>

int main() {
    // Fereastră 800x600 (notă: Vector2u + sufix 'u' pentru unsigned)
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "2D Golf");

    while (window.isOpen()) {
        // pollEvent() -> std::optional<sf::Event>
        while (auto event = window.pollEvent()) {
            // Închide fereastra la evenimentul Closed
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}
