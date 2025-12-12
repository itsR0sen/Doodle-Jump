#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>

namespace Utils
{
    // Collision detection
    bool isColliding(const sf::FloatRect &a, const sf::FloatRect &b);

    // Random number generation
    int randomInt(int min, int max);
    float randomFloat(float min, float max);

    // Clamping
    float clamp(float value, float min, float max);
    int clampInt(int value, int min, int max);

    // Text utilities
    sf::Text createText(const sf::Font &font, const std::string &str,
                        int size, sf::Color color, float x = 0, float y = 0);

    // Math utilities
    float distance(float x1, float y1, float x2, float y2);

    // File utilities
    bool fileExists(const std::string &filename);

    // High score management
    int loadHighScore();
    void saveHighScore(int score);
}

#endif