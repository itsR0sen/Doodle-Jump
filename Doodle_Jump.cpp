#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <cmath>
using namespace sf;

// ============================================================
// GAME CONSTANTS
// ============================================================
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 533;
const int PLATFORM_COUNT = 10;
const int PLATFORM_SPACING = 60; // Increased spacing for less crowding
const int PLAYER_WIDTH = 50;
const int PLAYER_HEIGHT = 70;
const int PLATFORM_WIDTH = 68;
const int PLATFORM_HEIGHT = 14;
const float GRAVITY = 0.2f;
const float JUMP_POWER = 11.0f;
const int PLAYER_SPEED = 4;
const int SCREEN_BOUNDARY_RIGHT = 350;
const int SCREEN_BOUNDARY_LEFT = 0;
const int CAMERA_THRESHOLD = 200;
const int SCORE_INCREMENT = 1;                 // Increased frequency of score updates
const Color HOVER_COLOR = Color(255, 200, 87); // Golden
const Color TEXT_COLOR = Color::White;
const Color SECONDARY_TEXT_COLOR = Color(200, 220, 255); // Light blue
const Color ACCENT_COLOR = Color(0, 255, 150);           // Bright green/cyan
const Color BUTTON_BG_COLOR = Color(40, 50, 80);         // Dark blue for buttons

// UI Layout constants
const int BUTTON_WIDTH = 120;
const int BUTTON_HEIGHT = 50;
const int BUTTON_SPACING = 20;
const int PADDING = 20;

struct point
{
    int x, y;
};

int loadHighScore()
{
    int highScore = 0;
    std::ifstream file("highscore.txt");
    if (file.is_open())
    {
        file >> highScore;
        file.close();
    }
    return highScore;
}

void saveHighScore(int score)
{
    std::ofstream file("highscore.txt");
    file << score;
    file.close();
}

// Helper function to create a styled button with outline effect
void drawStyledButton(RenderWindow &app, const std::string &label, const Font &font,
                      const Vector2f &position, const Vector2f &size,
                      bool isHovered, int fontSize)
{
    // Button background - show subtle background always, highlight on hover
    RectangleShape buttonBg(size);
    buttonBg.setPosition(position);
    buttonBg.setFillColor(isHovered ? Color(60, 70, 100) : Color(30, 40, 70));
    buttonBg.setOutlineThickness(2.0f);
    buttonBg.setOutlineColor(isHovered ? HOVER_COLOR : Color(100, 120, 150));
    app.draw(buttonBg);

    // Button text - immediate color change on hover
    Text buttonText(font);
    buttonText.setString(label);
    buttonText.setCharacterSize(fontSize);
    buttonText.setFillColor(isHovered ? HOVER_COLOR : TEXT_COLOR);

    // Properly center text in button area using local bounds
    FloatRect textBounds = buttonText.getLocalBounds();
    float textX = position.x + (size.x - textBounds.size.x) / 2 - textBounds.position.x;
    float textY = position.y + (size.y - textBounds.size.y) / 2 - textBounds.position.y;
    buttonText.setPosition(Vector2f(textX, textY));
    app.draw(buttonText);
}

int main()
{
    srand((unsigned int)time(0));

    // Window setup
    RenderWindow app(VideoMode(Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Doodle Jump");
    app.setFramerateLimit(60);
    app.setTitle("Doodle Jump - Jump High!");

    // Load textures
    Texture t1, t2, t3, resumeTex;
    t1.loadFromFile("images/background.png");
    t2.loadFromFile("images/platform.png");
    t3.loadFromFile("images/doodle.png");
    resumeTex.loadFromFile("images/resume.png");

    // Load font
    Font font;
    font.openFromFile("images/font.otf");

    // Sprites
    Sprite sBackground(t1), sPlat(t2), sPers(t3);
    Sprite resumeButton(resumeTex);

    // Button positions (centered)
    float playButtonWidth = resumeButton.getGlobalBounds().size.x;
    resumeButton.setPosition(Vector2f(WINDOW_WIDTH / 2 - playButtonWidth / 2, 200));

    // Pre-allocate all UI Text objects to avoid per-frame allocation
    Text titleText(font);
    titleText.setCharacterSize(52);
    titleText.setFillColor(ACCENT_COLOR);
    titleText.setString("DOODLE JUMP");

    Text subtitleText(font);
    subtitleText.setCharacterSize(16);
    subtitleText.setFillColor(SECONDARY_TEXT_COLOR);
    subtitleText.setString("Jump to the top!");

    Text instructionsText(font);
    instructionsText.setCharacterSize(14);
    instructionsText.setFillColor(ACCENT_COLOR);
    instructionsText.setString("Controls:");

    Text controlsText(font);
    controlsText.setCharacterSize(12);
    controlsText.setFillColor(SECONDARY_TEXT_COLOR);
    controlsText.setString("← → Arrow Keys to Move\nSPACE to Pause");

    Text hsLabelText(font);
    hsLabelText.setCharacterSize(14);
    hsLabelText.setFillColor(ACCENT_COLOR);
    hsLabelText.setString("Best Score");

    Text hsText(font);
    hsText.setCharacterSize(32);
    hsText.setFillColor(HOVER_COLOR);

    // Playing/Paused state text objects
    Text scoreTextShadow(font);
    scoreTextShadow.setCharacterSize(24);
    scoreTextShadow.setFillColor(Color(0, 0, 0, 150));

    Text scoreText(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(ACCENT_COLOR);

    Text scoreLabelText(font);
    scoreLabelText.setCharacterSize(12);
    scoreLabelText.setFillColor(SECONDARY_TEXT_COLOR);
    scoreLabelText.setString("SCORE");

    Text pausedText(font);
    pausedText.setCharacterSize(50);
    pausedText.setFillColor(HOVER_COLOR);
    pausedText.setString("PAUSED");

    Text resumeLabel(font);
    resumeLabel.setCharacterSize(12);
    resumeLabel.setFillColor(SECONDARY_TEXT_COLOR);
    resumeLabel.setString("Click to Resume");

    // Game Over state text objects
    Text gameOverText(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(Color(255, 100, 100));
    gameOverText.setString("GAME OVER");

    Text scoreLabel(font);
    scoreLabel.setCharacterSize(16);
    scoreLabel.setFillColor(SECONDARY_TEXT_COLOR);
    scoreLabel.setString("Your Score");

    Text finalScoreText(font);
    finalScoreText.setCharacterSize(48);
    finalScoreText.setFillColor(HOVER_COLOR);

    Text separator1(font);
    separator1.setCharacterSize(14);
    separator1.setFillColor(SECONDARY_TEXT_COLOR);
    separator1.setString("─────────────────");

    Text highScoreLabel(font);
    highScoreLabel.setCharacterSize(16);
    highScoreLabel.setFillColor(SECONDARY_TEXT_COLOR);
    highScoreLabel.setString("Best Score");

    Text highScoreText(font);
    highScoreText.setCharacterSize(36);
    highScoreText.setFillColor(ACCENT_COLOR);

    // Pre-allocate shape objects
    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(0, 0, 0, 180));

    RectangleShape pauseOverlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    pauseOverlay.setFillColor(Color(0, 0, 0, 200));

    // Game state
    enum GameState
    {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };
    GameState state = MENU;

    // Mouse position tracking for hover effect
    Vector2f mousePos(0, 0);

    // Button bounds storage for consistent hover detection
    FloatRect playButtonBounds;
    FloatRect pauseExitButtonBounds;
    FloatRect gameOverPlayAgainButtonBounds;
    FloatRect gameOverExitButtonBounds;

    // Score system
    int currentScore = 0;
    int highScore = loadHighScore();
    int maxHeight = WINDOW_HEIGHT;
    int minPlayerY = WINDOW_HEIGHT; // Track minimum Y position reached by player

    // Game objects
    point plat[PLATFORM_COUNT];
    bool platScored[PLATFORM_COUNT]; // Track which platforms have been scored
    int x = WINDOW_WIDTH / 2, y = WINDOW_HEIGHT - 145, h = CAMERA_THRESHOLD;
    float dx = 0, dy = 0;
    int frameCounter = 0; // For smooth animations

    // Initialize platforms
    auto initializePlatforms = [&]()
    {
        // Create platforms evenly spaced throughout the screen
        for (int i = 0; i < PLATFORM_COUNT; i++)
        {
            plat[i].x = rand() % (SCREEN_BOUNDARY_RIGHT - PLATFORM_WIDTH);
            plat[i].y = i * PLATFORM_SPACING;
            platScored[i] = false; // Reset scoring status
        }
        // Ensure there's always a platform directly beneath the starting position
        // Place a platform at the bottom where the player starts
        plat[9].x = WINDOW_WIDTH / 2 - PLATFORM_WIDTH / 2;
        plat[9].y = WINDOW_HEIGHT - 80;
        platScored[9] = false; // Reset starting platform scoring

        maxHeight = WINDOW_HEIGHT;
        minPlayerY = WINDOW_HEIGHT;
        currentScore = 0;
        frameCounter = 0;
    };

    initializePlatforms();

    while (app.isOpen())
    {
        frameCounter++;

        if (const auto event = app.pollEvent())
        {
            if (event->is<Event::Closed>())
                app.close();

            if (event->is<Event::KeyPressed>())
            {
                const auto *key = event->getIf<Event::KeyPressed>();
                if (key && key->code == Keyboard::Key::Space)
                {
                    if (state == PLAYING)
                    {
                        state = PAUSED;
                    }
                    else if (state == PAUSED)
                    {
                        state = PLAYING;
                    }
                }
                // ESC to return to menu from game over
                if (key && key->code == Keyboard::Key::Escape)
                {
                    if (state == GAME_OVER || state == PAUSED)
                    {
                        state = MENU;
                    }
                }
            }

            if (event->is<Event::MouseMoved>())
            {
                const auto *mouseMoved = event->getIf<Event::MouseMoved>();
                if (mouseMoved)
                {
                    mousePos = app.mapPixelToCoords(mouseMoved->position);
                }
            }

            if (event->is<Event::MouseButtonPressed>())
            {
                const auto *mouse = event->getIf<Event::MouseButtonPressed>();
                if (mouse)
                {
                    mousePos = app.mapPixelToCoords(mouse->position);

                    // Menu: Play button clicked
                    if (state == MENU)
                    {
                        if (playButtonBounds.contains(mousePos))
                        {
                            state = PLAYING;
                            initializePlatforms();
                            x = WINDOW_WIDTH / 2;
                            y = WINDOW_HEIGHT - 145;
                            dx = 0;
                            dy = 0;
                        }
                    }

                    // Paused: Resume button clicked
                    if (state == PAUSED)
                    {
                        FloatRect resumeButtonBounds = resumeButton.getGlobalBounds();
                        if (resumeButtonBounds.contains(mousePos))
                        {
                            state = PLAYING;
                        }
                    }

                    // Paused: Exit button clicked
                    if (state == PAUSED)
                    {
                        if (pauseExitButtonBounds.contains(mousePos))
                        {
                            app.close(); // Close the application
                        }
                    }

                    // Game Over: Play Again button clicked
                    if (state == GAME_OVER)
                    {
                        if (gameOverPlayAgainButtonBounds.contains(mousePos))
                        {
                            state = PLAYING;
                            initializePlatforms();
                            x = WINDOW_WIDTH / 2;
                            y = WINDOW_HEIGHT - 145;
                            dx = 0;
                            dy = 0;
                        }
                    }

                    // Game Over: Exit button clicked
                    if (state == GAME_OVER)
                    {
                        if (gameOverExitButtonBounds.contains(mousePos))
                        {
                            app.close(); // Close the application
                        }
                    }
                }
            }
        }

        // Game logic only runs when playing
        if (state == PLAYING)
        {
            // Smooth horizontal movement with acceleration
            if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            {
                x += PLAYER_SPEED;
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Left))
            {
                x -= PLAYER_SPEED;
            }

            // Keep doodle within screen bounds
            if (x < SCREEN_BOUNDARY_LEFT)
                x = SCREEN_BOUNDARY_LEFT;
            if (x > SCREEN_BOUNDARY_RIGHT)
                x = SCREEN_BOUNDARY_RIGHT;

            // Smooth gravity and falling
            dy += GRAVITY;
            y += (int)dy;

            // Collision detection with platforms
            for (int i = 0; i < PLATFORM_COUNT; i++)
            {
                if ((x + PLAYER_WIDTH / 2 > plat[i].x) &&
                    (x + PLAYER_WIDTH / 2 < plat[i].x + PLATFORM_WIDTH) && (y + PLAYER_HEIGHT > plat[i].y) &&
                    (y + PLAYER_HEIGHT < plat[i].y + PLATFORM_HEIGHT) &&
                    (dy > 0))
                {
                    dy = -JUMP_POWER;

                    // Award points if this platform hasn't been scored yet
                    if (!platScored[i])
                    {
                        currentScore += 10;   // Points per platform
                        platScored[i] = true; // Mark platform as scored
                    }
                }
            }

            // Game Over condition: fell off bottom
            if (y > WINDOW_HEIGHT)
            {
                state = GAME_OVER;
                if (currentScore > highScore)
                {
                    highScore = currentScore;
                    saveHighScore(highScore);
                }
            }

            // Camera follow: when player goes up, scroll platforms
            if (y < h)
            {
                for (int i = 0; i < PLATFORM_COUNT; i++)
                {
                    y = h;
                    plat[i].y = plat[i].y - (int)dy;

                    // Recycle platform when it goes off screen
                    if (plat[i].y > WINDOW_HEIGHT)
                    {
                        plat[i].y = -PLATFORM_HEIGHT;
                        platScored[i] = false; // Reset scoring status for recycled platform

                        // Find the highest platform to maintain spacing
                        int maxPlatformY = 0;
                        for (int j = 0; j < PLATFORM_COUNT; j++)
                        {
                            if (plat[j].y < 0)
                                maxPlatformY = std::min(maxPlatformY, plat[j].y);
                        }

                        // Spawn new platform below the highest one with consistent spacing
                        plat[i].y = maxPlatformY - PLATFORM_SPACING;
                        plat[i].x = rand() % (SCREEN_BOUNDARY_RIGHT - PLATFORM_WIDTH);
                    }
                }
            }
        }

        // Rendering
        app.clear(Color(20, 20, 40));
        app.draw(sBackground);

        if (state == MENU)
        {
            // Title
            float titleX = (WINDOW_WIDTH - titleText.getGlobalBounds().size.x) / 2;
            titleText.setPosition(Vector2f(titleX, 40));
            app.draw(titleText);

            // Subtitle
            float subtitleX = (WINDOW_WIDTH - subtitleText.getGlobalBounds().size.x) / 2;
            subtitleText.setPosition(Vector2f(subtitleX, 110));
            app.draw(subtitleText);

            // Play button - centered
            float playButtonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
            float playButtonY = 170;
            playButtonBounds = FloatRect(Vector2f(playButtonX, playButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
            bool playHovered = playButtonBounds.contains(mousePos);
            drawStyledButton(app, "PLAY", font, Vector2f(playButtonX, playButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), playHovered, 36);

            // Instructions
            instructionsText.setPosition(Vector2f(PADDING, 280));
            app.draw(instructionsText);

            controlsText.setPosition(Vector2f(PADDING, 310));
            app.draw(controlsText);

            // High score display - bottom
            hsLabelText.setPosition(Vector2f(PADDING, 450));
            app.draw(hsLabelText);

            hsText.setString(std::to_string(highScore));
            hsText.setPosition(Vector2f(PADDING, 470));
            app.draw(hsText);
        }
        else if (state == PLAYING || state == PAUSED)
        {
            // Draw platforms first
            for (int i = 0; i < PLATFORM_COUNT; i++)
            {
                sPlat.setPosition(Vector2f(plat[i].x, plat[i].y));
                app.draw(sPlat);
            }

            // Draw player on top of platforms
            sPers.setPosition(Vector2f(x, y));
            app.draw(sPers);

            // Draw current score with shadow - positioned in top left with better styling
            scoreTextShadow.setPosition(Vector2f(13, 13));
            scoreTextShadow.setString(std::to_string(currentScore));
            app.draw(scoreTextShadow);

            scoreText.setPosition(Vector2f(10, 10));
            scoreText.setString(std::to_string(currentScore));
            app.draw(scoreText);

            // Score label
            scoreLabelText.setPosition(Vector2f(10, 40));
            app.draw(scoreLabelText);

            if (state == PAUSED)
            {
                // Semi-transparent overlay
                app.draw(overlay);

                // Paused text - centered
                float pausedX = (WINDOW_WIDTH - pausedText.getGlobalBounds().size.x) / 2;
                pausedText.setPosition(Vector2f(pausedX, 80));
                app.draw(pausedText);

                // Resume button
                app.draw(resumeButton);

                // Resume button label
                float resumeLabelX = (WINDOW_WIDTH - resumeLabel.getGlobalBounds().size.x) / 2;
                resumeLabel.setPosition(Vector2f(resumeLabelX, 280));
                app.draw(resumeLabel);

                // Exit button - centered below
                float exitButtonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
                float exitButtonY = 330;
                pauseExitButtonBounds = FloatRect(Vector2f(exitButtonX, exitButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
                bool exitHovered = pauseExitButtonBounds.contains(mousePos);
                drawStyledButton(app, "EXIT", font, Vector2f(exitButtonX, exitButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), exitHovered, 32);
            }
        }
        else if (state == GAME_OVER)
        {
            // Semi-transparent overlay
            app.draw(pauseOverlay);

            // Game Over text - centered
            float gameOverX = (WINDOW_WIDTH - gameOverText.getGlobalBounds().size.x) / 2;
            gameOverText.setPosition(Vector2f(gameOverX, 50));
            app.draw(gameOverText);

            // Score section - centered
            float scoreLabelX = (WINDOW_WIDTH - scoreLabel.getGlobalBounds().size.x) / 2;
            scoreLabel.setPosition(Vector2f(scoreLabelX, 130));
            app.draw(scoreLabel);

            finalScoreText.setString(std::to_string(currentScore));
            float scoreX = (WINDOW_WIDTH - finalScoreText.getGlobalBounds().size.x) / 2;
            finalScoreText.setPosition(Vector2f(scoreX, 160));
            app.draw(finalScoreText);

            // Separator line effect
            float sep1X = (WINDOW_WIDTH - separator1.getGlobalBounds().size.x) / 2;
            separator1.setPosition(Vector2f(sep1X, 225));
            app.draw(separator1);

            // High score section - centered
            float hsLabelX = (WINDOW_WIDTH - highScoreLabel.getGlobalBounds().size.x) / 2;
            highScoreLabel.setPosition(Vector2f(hsLabelX, 250));
            app.draw(highScoreLabel);

            highScoreText.setString(std::to_string(highScore));
            float hsX = (WINDOW_WIDTH - highScoreText.getGlobalBounds().size.x) / 2;
            highScoreText.setPosition(Vector2f(hsX, 275));
            app.draw(highScoreText);

            // Play Again button - centered
            float playAgainButtonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
            float playAgainButtonY = 350;
            gameOverPlayAgainButtonBounds = FloatRect(Vector2f(playAgainButtonX, playAgainButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
            bool playAgainHovered = gameOverPlayAgainButtonBounds.contains(mousePos);
            drawStyledButton(app, "PLAY AGAIN", font, Vector2f(playAgainButtonX, playAgainButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), playAgainHovered, 22);

            // Exit button - centered below
            float exitButtonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
            float exitButtonY = 410;
            gameOverExitButtonBounds = FloatRect(Vector2f(exitButtonX, exitButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
            bool exitHovered = gameOverExitButtonBounds.contains(mousePos);
            drawStyledButton(app, "EXIT", font, Vector2f(exitButtonX, exitButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), exitHovered, 32);
        }

        app.display();
    }

    return 0;
}