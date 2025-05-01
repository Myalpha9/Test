#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

int main() {
    // Get half desktop resolution for better performance
    unsigned int width = sf::VideoMode::getDesktopMode().width / 2;
    unsigned int height = sf::VideoMode::getDesktopMode().height / 2;

    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Explorer");
    window.setFramerateLimit(30);

    // Load font for text display
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        return -1; // Make sure you have a valid .ttf font in the project folder
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 10);

    // Create ComplexPlane object
    ComplexPlane complexPlane(width, height);

    // Main loop
    while (window.isOpen()) {
        // Handle input
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
                    complexPlane.zoomIn();
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
                    complexPlane.zoomOut();
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                complexPlane.setMouseLocation({ event.mouseMove.x, event.mouseMove.y });
            }
        }

        // Check keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        // Update the complex plane rendering if needed
        complexPlane.updateRender();

        // Update onscreen text
        complexPlane.loadText(text);

        // Draw everything
        window.clear();
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }

    return 0;
}
