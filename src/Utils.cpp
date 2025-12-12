#include "Utils.h"
#include <iostream>
#include <fstream>

namespace Utils
{
    // Static random number generator
    static std::random_device rd;
    static std::mt19937 gen(rd());

    bool isColliding(const sf::FloatRect &a, const sf::FloatRect &b)
    {
        return a.intersects(b);
    }

    int randomInt(int min, int max)
    {
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    float randomFloat(float min, float max)
    {
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    float clamp(float value, float min, float max)
    {
        if (value < min)
            return min;
        if (value > max)
            return max;
        return value;
    }

    int clampInt(int value, int min, int max)
    {
        if (value < min)
            return min;
        if (value > max)
            return max;
        return value;
    }

    sf::Text createText(const sf::Font &font, const std::string &str,
                        int size, sf::Color color, float x, float y)
    {
        sf::Text text(str, font, size);
        text.setFillColor(color);
        text.setPosition(x, y);
        return text;
    }

    float distance(float x1, float y1, float x2, float y2)
    {
        float dx = x2 - x1;
        float dy = y2 - y1;
        return std::sqrt(dx * dx + dy * dy);
    }

    bool fileExists(const std::string &filename)
    {
        std::ifstream file(filename);
        return file.good();
    }

    int loadHighScore()
    {
        std::ifstream file("highscore.txt");
        if (!file.is_open())
            return 0;

        int score = 0;
        file >> score;
        file.close();
        return score;
    }

    void saveHighScore(int score)
    {
        std::ofstream file("highscore.txt");
        if (file.is_open())
        {
            file << score;
            file.close();
        }
    }
}