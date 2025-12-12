#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player
{
private:
    sf::Sprite sprite;
    float x, y;
    float speed;
    int health;
    int maxHealth;

public:
    Player(sf::Texture &texture, float startX, float startY);

    // Update and drawing
    void update(float deltaTime, int screenWidth, int screenHeight);
    void draw(sf::RenderWindow &window);

    // Getters
    sf::FloatRect getBounds() const;
    float getX() const { return x; }
    float getY() const { return y; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    bool isAlive() const { return health > 0; }

    // Setters
    void setHealth(int h);
    void takeDamage(int damage = 1);
    void heal(int amount = 1);

    // Position
    void setPosition(float px, float py);
};

#endif