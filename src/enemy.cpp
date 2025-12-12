#include "Enemy.h"
#include "Utils.h"

// ===== ENEMY CLASS =====
Enemy::Enemy(sf::Texture &texture, float startX, float startY, float spd, int laneNum)
    : x(startX), y(startY), speed(spd), lane(laneNum)
{
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void Enemy::update(float deltaTime)
{
    y += speed * deltaTime;
    sprite.setPosition(x, y);
}

void Enemy::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

sf::FloatRect Enemy::getBounds() const
{
    return sprite.getGlobalBounds();
}

bool Enemy::isOffScreen(int screenHeight) const
{
    return y > screenHeight + 100;
}

// ===== ENEMY MANAGER CLASS =====
EnemyManager::EnemyManager(sf::Texture &texture, int screenW, int screenH)
    : enemyTexture(texture), spawnTimer(0), spawnInterval(1.5f),
      maxSpeed(200.0f), screenWidth(screenW), screenHeight(screenH)
{
}

EnemyManager::~EnemyManager()
{
    clear();
}

void EnemyManager::update(float deltaTime)
{
    // Spawn new enemies
    spawnTimer -= deltaTime;
    if (spawnTimer <= 0)
    {
        spawnEnemy(maxSpeed);
        spawnTimer = spawnInterval;
    }

    // Update all enemies
    for (auto enemy : enemies)
    {
        enemy->update(deltaTime);
    }

    // Remove off-screen enemies
    for (int i = enemies.size() - 1; i >= 0; i--)
    {
        if (enemies[i]->isOffScreen(screenHeight))
        {
            delete enemies[i];
            enemies.erase(enemies.begin() + i);
        }
    }
}

void EnemyManager::draw(sf::RenderWindow &window)
{
    for (auto enemy : enemies)
    {
        enemy->draw(window);
    }
}

void EnemyManager::spawnEnemy(float currentSpeed)
{
    // Random lane: 0 (left), 1 (middle), 2 (right)
    int lane = Utils::randomInt(0, 2);
    float xPos;

    switch (lane)
    {
    case 0:
        xPos = screenWidth / 6.0f;
        break; // Left lane
    case 1:
        xPos = screenWidth / 2.0f;
        break; // Middle lane
    case 2:
        xPos = screenWidth * 5.0f / 6.0f;
        break; // Right lane
    default:
        xPos = screenWidth / 2.0f;
    }

    enemies.push_back(new Enemy(enemyTexture, xPos, -50, currentSpeed, lane));
}

void EnemyManager::clear()
{
    for (auto enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();
}