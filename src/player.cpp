#include "Player.h"
#include "Game.h"

Player::Player(sf::Texture &texture, float startX, float startY)
    : x(startX), y(startY), speed(Game::PLAYER_SPEED),
      health(Game::INITIAL_HEALTH), maxHealth(Game::INITIAL_HEALTH)
{
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void Player::update(float deltaTime, int screenWidth, int screenHeight)
{
    // Handle input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        x -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        x += speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        y -= speed * 0.5f * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        y += speed * 0.5f * deltaTime;
    }

    // Keep in bounds
    float spriteWidth = sprite.getLocalBounds().width;
    float spriteHeight = sprite.getLocalBounds().height;

    if (x < 0)
        x = 0;
    if (x + spriteWidth > screenWidth)
        x = screenWidth - spriteWidth;
    if (y < 0)
        y = 0;
    if (y + spriteHeight > screenHeight)
        y = screenHeight - spriteHeight;

    sprite.setPosition(x, y);
}

void Player::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

sf::FloatRect Player::getBounds() const
{
    return sprite.getGlobalBounds();
}

void Player::setHealth(int h)
{
    health = h;
    if (health > maxHealth)
        health = maxHealth;
    if (health < 0)
        health = 0;
}

void Player::takeDamage(int damage)
{
    health -= damage;
    if (health < 0)
        health = 0;
}

void Player::heal(int amount)
{
    health += amount;
    if (health > maxHealth)
        health = maxHealth;
}

void Player::setPosition(float px, float py)
{
    x = px;
    y = py;
    sprite.setPosition(x, y);
}