#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>

class Enemy
{
private:
    sf::Sprite sprite;
    float x, y;
    float speed;
    int lane; // 0 = left, 1 = middle, 2 = right

public:
    Enemy(sf::Texture &texture, float startX, float startY, float spd, int laneNum);

    // Update and drawing
    void update(float deltaTime);
    void draw(sf::RenderWindow &window);

    // Getters
    sf::FloatRect getBounds() const;
    float getX() const { return x; }
    float getY() const { return y; }
    float getSpeed() const { return speed; }
    int getLane() const { return lane; }
    bool isOffScreen(int screenHeight) const;

    // Setters
    void setSpeed(float s) { speed = s; }
};

class EnemyManager
{
private:
    std::vector<Enemy *> enemies;
    sf::Texture &enemyTexture;
    float spawnTimer;
    float spawnInterval;
    float maxSpeed;
    int screenWidth;
    int screenHeight;

public:
    EnemyManager(sf::Texture &texture, int screenW, int screenH);
    ~EnemyManager();

    // Management
    void update(float deltaTime);
    void draw(sf::RenderWindow &window);
    void spawnEnemy(float currentSpeed);
    void clear();

    // Getters
    std::vector<Enemy *> &getEnemies() { return enemies; }
    float getSpawnInterval() const { return spawnInterval; }

    // Difficulty
    void increaseSpeed(float amount) { maxSpeed += amount; }
    void setSpeed(float speed) { maxSpeed = speed; }
    void setSpawnInterval(float interval) { spawnInterval = interval; }
    void decreaseSpawnInterval(float amount)
    {
        if (spawnInterval > 0.5f)
            spawnInterval -= amount;
    }
};

#endif