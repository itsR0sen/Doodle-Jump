#include "Game.h"
#include "Utils.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Game::Game()
    : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Car Dodge Game"),
      player(nullptr), enemyManager(nullptr), sceneManager(nullptr),
      score(0), highScore(0), survivalTime(0), gameTime(0),
      difficultyMultiplier(1.0f), isRunning(true)
{
    window.setFramerateLimit(FPS);
    sceneManager = new SceneManager();
    highScore = Utils::loadHighScore();
}

Game::~Game()
{
    cleanup();
}

bool Game::initialize()
{
    if (!loadAssets())
    {
        std::cerr << "Failed to load assets!\n";
        return false;
    }

    // Create game objects
    player = new Player(playerTexture, SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT - 100);
    enemyManager = new EnemyManager(enemyTexture, SCREEN_WIDTH, SCREEN_HEIGHT);

    return true;
}

void Game::cleanup()
{
    if (player)
        delete player;
    if (enemyManager)
        delete enemyManager;
    if (sceneManager)
        delete sceneManager;
}

bool Game::loadAssets()
{
    // Load textures
    if (!playerTexture.loadFromFile("Assets/player.png"))
    {
        std::cerr << "Error loading Assets/player.png\n";
        return false;
    }
    if (!enemyTexture.loadFromFile("Assets/enemy.png"))
    {
        std::cerr << "Error loading Assets/enemy.png\n";
        return false;
    }

    // Load font (optional - will display in console if not found)
    if (!font.loadFromFile("Assets/arial.ttf"))
    {
        std::cerr << "Warning: Font not loaded. UI text will not display.\n";
    }

    return true;
}

void Game::run()
{
    if (!initialize())
    {
        return;
    }

    sf::Clock clock;

    while (window.isOpen() && isRunning)
    {
        float deltaTime = clock.restart().asSeconds();

        handleInput();
        update(deltaTime);
        render();
    }

    cleanup();
}

void Game::handleInput()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            isRunning = false;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                isRunning = false;
            }

            // Scene-specific input
            switch (sceneManager->getCurrentScene())
            {
            case SCENE_MENU:
                if (event.key.code == sf::Keyboard::Space)
                {
                    goToPlay();
                }
                break;

            case SCENE_GAMEOVER:
                if (event.key.code == sf::Keyboard::Space)
                {
                    goToMenu();
                }
                break;

            default:
                break;
            }
        }
    }
}

void Game::update(float deltaTime)
{
    sceneManager->updateScene();

    switch (sceneManager->getCurrentScene())
    {
    case SCENE_MENU:
        // Menu state - nothing to update
        break;

    case SCENE_PLAY:
        if (!player->isAlive())
        {
            goToGameOver();
            break;
        }

        // Update game
        player->update(deltaTime, SCREEN_WIDTH, SCREEN_HEIGHT);
        enemyManager->update(deltaTime);

        handleCollisions();
        updateDifficulty(deltaTime);

        survivalTime += deltaTime;
        score += static_cast<int>(10 * difficultyMultiplier * deltaTime);

        break;

    case SCENE_GAMEOVER:
        // Game over state - nothing to update
        break;
    }
}

void Game::render()
{
    window.clear(sf::Color::Black);

    switch (sceneManager->getCurrentScene())
    {
    case SCENE_MENU:
        drawMenuScene();
        break;

    case SCENE_PLAY:
        // Draw game
        player->draw(window);
        enemyManager->draw(window);

        // Draw UI
        drawGameUI();
        break;

    case SCENE_GAMEOVER:
        drawGameOverScene();
        break;
    }

    window.display();
}

void Game::startNewGame()
{
    if (player)
        delete player;
    if (enemyManager)
        delete enemyManager;

    player = new Player(playerTexture, SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT - 100);
    enemyManager = new EnemyManager(enemyTexture, SCREEN_WIDTH, SCREEN_HEIGHT);

    score = 0;
    survivalTime = 0;
    gameTime = 0;
    difficultyMultiplier = 1.0f;
}

void Game::handleCollisions()
{
    sf::FloatRect playerBounds = player->getBounds();

    for (auto &enemy : enemyManager->getEnemies())
    {
        if (Utils::isColliding(playerBounds, enemy->getBounds()))
        {
            player->takeDamage(1);
            enemy->setSpeed(0); // Disable enemy to avoid multiple hits
        }
    }
}

void Game::updateDifficulty(float deltaTime)
{
    gameTime += deltaTime;

    // Increase difficulty over time
    difficultyMultiplier = 1.0f + (gameTime * 0.2f);

    // Increase enemy speed
    float newSpeed = Game::INITIAL_ENEMY_SPEED * difficultyMultiplier;
    enemyManager->setSpeed(newSpeed);

    // Decrease spawn interval
    float newInterval = Game::INITIAL_SPAWN_INTERVAL / (1.0f + gameTime * 0.1f);
    enemyManager->setSpawnInterval(newInterval);
}

void Game::drawGameUI()
{
    if (!font.getInfo().family.empty())
    {
        // Draw score
        sf::Text scoreText = Utils::createText(font, "Score: " + std::to_string(score),
                                               20, sf::Color::White, 10, 10);
        window.draw(scoreText);

        // Draw health
        std::string healthStr = "Health: ";
        for (int i = 0; i < player->getHealth(); i++)
        {
            healthStr += "█";
        }
        sf::Text healthText = Utils::createText(font, healthStr,
                                                20, sf::Color::Green, 10, 40);
        window.draw(healthText);

        // Draw time
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << survivalTime;
        sf::Text timeText = Utils::createText(font, "Time: " + oss.str() + "s",
                                              20, sf::Color::Yellow, 10, 70);
        window.draw(timeText);

        // Draw difficulty
        std::ostringstream diffStr;
        diffStr << std::fixed << std::setprecision(1) << difficultyMultiplier;
        sf::Text diffText = Utils::createText(font, "Difficulty: " + diffStr.str() + "x",
                                              18, sf::Color::Red, SCREEN_WIDTH - 220, 10);
        window.draw(diffText);
    }
    else
    {
        // Console output if font not loaded
        static int frameCount = 0;
        if (frameCount % 60 == 0)
        {
            std::cout << "Score: " << score << " | Health: " << player->getHealth()
                      << " | Time: " << survivalTime << "\n";
        }
        frameCount++;
    }
}

void Game::drawMenuScene()
{
    if (!font.getInfo().family.empty())
    {
        // Title
        sf::Text title = Utils::createText(font, "CAR DODGE", 60, sf::Color::White,
                                           SCREEN_WIDTH / 2 - 150, 100);
        window.draw(title);

        // Instructions
        sf::Text instructions = Utils::createText(font,
                                                  "Avoid incoming cars to survive!\n\n"
                                                  "Controls:\n"
                                                  "← → : Move left/right\n"
                                                  "↑ ↓ : Move up/down\n\n"
                                                  "Press SPACE to start",
                                                  24, sf::Color::Green, 150, 250);
        window.draw(instructions);

        // High score
        sf::Text hsText = Utils::createText(font, "High Score: " + std::to_string(highScore),
                                            20, sf::Color::Yellow, 10, SCREEN_HEIGHT - 30);
        window.draw(hsText);
    }
}

void Game::drawGameOverScene()
{
    if (!font.getInfo().family.empty())
    {
        // Game Over title
        sf::Text gameOverText = Utils::createText(font, "GAME OVER", 60, sf::Color::Red,
                                                  SCREEN_WIDTH / 2 - 180, 100);
        window.draw(gameOverText);

        // Final score
        sf::Text scoreText = Utils::createText(font, "Score: " + std::to_string(score),
                                               40, sf::Color::White,
                                               SCREEN_WIDTH / 2 - 120, 200);
        window.draw(scoreText);

        // Survival time
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << survivalTime;
        sf::Text timeText = Utils::createText(font, "Survived: " + oss.str() + " seconds",
                                              30, sf::Color::Yellow,
                                              SCREEN_WIDTH / 2 - 180, 270);
        window.draw(timeText);

        // High score if beaten
        if (score > highScore)
        {
            sf::Text newHS = Utils::createText(font, "NEW HIGH SCORE!",
                                               30, sf::Color::Green,
                                               SCREEN_WIDTH / 2 - 140, 350);
            window.draw(newHS);
            highScore = score;
            Utils::saveHighScore(highScore);
        }

        // Restart instruction
        sf::Text restartText = Utils::createText(font, "Press SPACE to return to menu",
                                                 24, sf::Color::Cyan,
                                                 SCREEN_WIDTH / 2 - 180, 450);
        window.draw(restartText);
    }
}