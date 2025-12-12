#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "SceneManager.h"

class Game
{
public:
    // Window constants
    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 600;
    static constexpr int FPS = 60;

    // Game constants
    static constexpr float PLAYER_SPEED = 400.0f;
    static constexpr int INITIAL_HEALTH = 3;
    static constexpr float INITIAL_ENEMY_SPEED = 200.0f;
    static constexpr float INITIAL_SPAWN_INTERVAL = 1.5f;

private:
    // Window
    sf::RenderWindow window;
    sf::Clock clock;

    // Assets
    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Font font;

    // Game objects
    Player *player;
    EnemyManager *enemyManager;
    SceneManager *sceneManager;

    // Game state
    int score;
    int highScore;
    float survivalTime;
    float gameTime;
    float difficultyMultiplier;
    bool isRunning;

public:
    Game();
    ~Game();

    // Main game loop
    void run();

    // Initialize and cleanup
    bool initialize();
    void cleanup();

    // Game methods
    void handleInput();
    void update(float deltaTime);
    void render();

    // Gameplay
    void startNewGame();
    void handleCollisions();
    void updateDifficulty(float deltaTime);

    // Getters
    int getScore() const { return score; }
    int getHighScore() const { return highScore; }
    float getSurvivalTime() const { return survivalTime; }
    Player *getPlayer() const { return player; }
    EnemyManager *getEnemyManager() const { return enemyManager; }

    // Scene switching
    void goToMenu() { sceneManager->setScene(SCENE_MENU); }
    void goToPlay()
    {
        startNewGame();
        sceneManager->setScene(SCENE_PLAY);
    }
    void goToGameOver() { sceneManager->setScene(SCENE_GAMEOVER); }

private:
    bool loadAssets();
    void drawGameUI();
    void drawMenuScene();
    void drawGameOverScene();
};

#endif