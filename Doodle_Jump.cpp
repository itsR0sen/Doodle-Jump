#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <cmath>
using namespace sf;

// Game constants
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 533;
const int PLATFORM_COUNT = 10;
const int PLATFORM_SPACING = 60;
const int MIN_PLATFORMS = 4;
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
const int SCORE_INCREMENT = 1;
const int DIFFICULTY_SCORE_THRESHOLD = 100;
const Color HOVER_COLOR = Color(255, 200, 87);
const Color TEXT_COLOR = Color::White;
const Color SECONDARY_TEXT_COLOR = Color(200, 220, 255);
const Color ACCENT_COLOR = Color(0, 255, 150);
const Color BUTTON_BG_COLOR = Color(40, 50, 80);

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

void drawStyledButton(RenderWindow &app, const std::string &label, const Font &font,
                      const Vector2f &position, const Vector2f &size,
                      bool isHovered, int fontSize)
{
    RectangleShape buttonBg(size);
    buttonBg.setPosition(position);
    buttonBg.setFillColor(isHovered ? Color(60, 70, 100) : Color(30, 40, 70));
    buttonBg.setOutlineThickness(2.0f);
    buttonBg.setOutlineColor(isHovered ? HOVER_COLOR : Color(100, 120, 150));
    app.draw(buttonBg);

    Text buttonText(font);
    buttonText.setString(label);
    buttonText.setCharacterSize(fontSize);
    buttonText.setFillColor(isHovered ? HOVER_COLOR : TEXT_COLOR);

    FloatRect textBounds = buttonText.getLocalBounds();
    float textX = position.x + (size.x - textBounds.size.x) / 2 - textBounds.position.x;
    float textY = position.y + (size.y - textBounds.size.y) / 2 - textBounds.position.y;
    buttonText.setPosition(Vector2f(textX, textY));
    app.draw(buttonText);
}

int main()
{
    srand((unsigned int)time(0));

    RenderWindow app(VideoMode(Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Doodle Jump");
    app.setFramerateLimit(60);
    app.setTitle("Doodle Jump - Jump High!");

    Texture t1, t2, t3, resumeTex;
    t1.loadFromFile("images/background.png");
    t2.loadFromFile("images/platform.png");
    t3.loadFromFile("images/doodle.png");
    resumeTex.loadFromFile("images/resume.png");

    Font font;
    font.openFromFile("images/font.otf");

    Sprite sBackground(t1), sPlat(t2), sPers(t3);
    Sprite resumeButton(resumeTex);

    float playButtonWidth = resumeButton.getGlobalBounds().size.x;
    resumeButton.setPosition(Vector2f(WINDOW_WIDTH / 2 - playButtonWidth / 2, 200));

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
    controlsText.setString("LEFT/RIGHT Arrow Keys\nSPACE to Pause");

    Text hsLabelText(font);
    hsLabelText.setCharacterSize(14);
    hsLabelText.setFillColor(ACCENT_COLOR);
    hsLabelText.setString("Best Score");

    Text hsText(font);
    hsText.setCharacterSize(32);
    hsText.setFillColor(HOVER_COLOR);

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
    separator1.setString("_____________________________________________________________________");

    Text highScoreLabel(font);
    highScoreLabel.setCharacterSize(16);
    highScoreLabel.setFillColor(SECONDARY_TEXT_COLOR);
    highScoreLabel.setString("Best Score");

    Text highScoreText(font);
    highScoreText.setCharacterSize(36);
    highScoreText.setFillColor(ACCENT_COLOR);

    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(0, 0, 0, 180));

    RectangleShape pauseOverlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    pauseOverlay.setFillColor(Color(0, 0, 0, 200));

    enum GameState
    {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };
    GameState state = MENU;

    Vector2f mousePos(0, 0);

    // Button bounds used for hit detection across different game states
    FloatRect playButtonBounds;
    FloatRect pauseExitButtonBounds;
    FloatRect gameOverPlayAgainButtonBounds;
    FloatRect gameOverExitButtonBounds;

    int currentScore = 0;
    int highScore = loadHighScore();
    int maxHeight = WINDOW_HEIGHT;
    int minPlayerY = WINDOW_HEIGHT;
    int activePlatforms = PLATFORM_COUNT;
    int lastDifficultyLevel = 0;
    int lastPlatformY = WINDOW_HEIGHT - 80;

    point plat[PLATFORM_COUNT];
    bool platScored[PLATFORM_COUNT];
    int x = WINDOW_WIDTH / 2, y = WINDOW_HEIGHT - 145, h = CAMERA_THRESHOLD;
    float dx = 0, dy = 0;
    int frameCounter = 0;

    auto initializePlatforms = [&]()
    {
        // Difficulty increases as score grows - fewer platforms available
        activePlatforms = PLATFORM_COUNT - (currentScore / DIFFICULTY_SCORE_THRESHOLD);
        if (activePlatforms < MIN_PLATFORMS)
        {
            activePlatforms = MIN_PLATFORMS;
        }

        for (int i = 0; i < PLATFORM_COUNT; i++)
        {
            if (i < activePlatforms)
            {
                plat[i].x = rand() % (SCREEN_BOUNDARY_RIGHT - PLATFORM_WIDTH);
                plat[i].y = i * PLATFORM_SPACING;
                platScored[i] = false;
            }
            else
            {
                // Disable unused platforms by placing off-screen
                plat[i].y = -PLATFORM_HEIGHT * 2;
                platScored[i] = false;
            }
        }

        // Always place starting platform at player spawn location
        plat[activePlatforms - 1].x = WINDOW_WIDTH / 2 - PLATFORM_WIDTH / 2;
        plat[activePlatforms - 1].y = WINDOW_HEIGHT - 80;

        maxHeight = WINDOW_HEIGHT;
        minPlayerY = WINDOW_HEIGHT;
        frameCounter = 0;
        lastPlatformY = WINDOW_HEIGHT - 80;
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

                    if (state == MENU)
                    {
                        if (playButtonBounds.contains(mousePos))
                        {
                            state = PLAYING;
                            currentScore = 0;
                            initializePlatforms();
                            x = WINDOW_WIDTH / 2;
                            y = WINDOW_HEIGHT - 145;
                            dx = 0;
                            dy = 0;
                        }
                    }

                    if (state == PAUSED)
                    {
                        FloatRect resumeButtonBounds = resumeButton.getGlobalBounds();
                        if (resumeButtonBounds.contains(mousePos))
                        {
                            state = PLAYING;
                        }
                    }

                    if (state == PAUSED)
                    {
                        if (pauseExitButtonBounds.contains(mousePos))
                        {
                            app.close();
                        }
                    }

                    if (state == GAME_OVER)
                    {
                        if (gameOverPlayAgainButtonBounds.contains(mousePos))
                        {
                            state = PLAYING;
                            currentScore = 0;
                            initializePlatforms();
                            x = WINDOW_WIDTH / 2;
                            y = WINDOW_HEIGHT - 145;
                            dx = 0;
                            dy = 0;
                        }
                    }

                    if (state == GAME_OVER)
                    {
                        if (gameOverExitButtonBounds.contains(mousePos))
                        {
                            app.close();
                        }
                    }
                }
            }
        }

        if (state == PLAYING)
        {
            if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            {
                x += PLAYER_SPEED;
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Left))
            {
                x -= PLAYER_SPEED;
            }

            // Wrap player position within horizontal bounds
            if (x < SCREEN_BOUNDARY_LEFT)
                x = SCREEN_BOUNDARY_LEFT;
            if (x > SCREEN_BOUNDARY_RIGHT)
                x = SCREEN_BOUNDARY_RIGHT;

            dy += GRAVITY;
            y += (int)dy;

            // Check collision with each platform
            for (int i = 0; i < activePlatforms; i++)
            {
                if ((x + PLAYER_WIDTH / 2 > plat[i].x) &&
                    (x + PLAYER_WIDTH / 2 < plat[i].x + PLATFORM_WIDTH) && (y + PLAYER_HEIGHT > plat[i].y) &&
                    (y + PLAYER_HEIGHT < plat[i].y + PLATFORM_HEIGHT) &&
                    (dy > 0))
                {
                    dy = -JUMP_POWER;

                    // Only award points once per platform touch
                    if (!platScored[i])
                    {
                        // Longer jumps are rewarded with more points
                        int jumpDistance = lastPlatformY - plat[i].y;

                        if (jumpDistance >= 80)
                        {
                            currentScore += 2;
                        }
                        else
                        {
                            currentScore += 1;
                        }

                        lastPlatformY = plat[i].y;
                        platScored[i] = true;

                        // Adjust difficulty: reduce available platforms as score increases
                        int currentDifficultyLevel = currentScore / DIFFICULTY_SCORE_THRESHOLD;
                        int newActivePlatforms = PLATFORM_COUNT - currentDifficultyLevel;
                        if (newActivePlatforms < MIN_PLATFORMS)
                        {
                            newActivePlatforms = MIN_PLATFORMS;
                        }

                        activePlatforms = newActivePlatforms;
                    }
                }
            }

            // End game when player falls off screen
            if (y > WINDOW_HEIGHT)
            {
                state = GAME_OVER;
                if (currentScore > highScore)
                {
                    highScore = currentScore;
                    saveHighScore(highScore);
                }
            }

            // Camera follows player upward - scroll platforms down relative to camera
            if (y < h)
            {
                for (int i = 0; i < activePlatforms; i++)
                {
                    y = h;
                    plat[i].y = plat[i].y - (int)dy;

                    // Recycle platform when it scrolls off bottom
                    if (plat[i].y > WINDOW_HEIGHT)
                    {
                        plat[i].y = -PLATFORM_HEIGHT;
                        platScored[i] = false;

                        // Find highest platform to maintain consistent spacing
                        int maxPlatformY = 0;
                        for (int j = 0; j < activePlatforms; j++)
                        {
                            if (plat[j].y < 0)
                                maxPlatformY = std::min(maxPlatformY, plat[j].y);
                        }

                        // Spawn new platform at top
                        plat[i].y = maxPlatformY - PLATFORM_SPACING;
                        plat[i].x = rand() % (SCREEN_BOUNDARY_RIGHT - PLATFORM_WIDTH);
                    }
                }
            }
        }

        app.clear(Color(20, 20, 40));
        app.draw(sBackground);

        if (state == MENU)
        {
            float titleX = (WINDOW_WIDTH - titleText.getGlobalBounds().size.x) / 2;
            titleText.setPosition(Vector2f(titleX, 40));
            app.draw(titleText);

            float subtitleX = (WINDOW_WIDTH - subtitleText.getGlobalBounds().size.x) / 2;
            subtitleText.setPosition(Vector2f(subtitleX, 110));
            app.draw(subtitleText);

            float playButtonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
            float playButtonY = 170;
            playButtonBounds = FloatRect(Vector2f(playButtonX, playButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
            bool playHovered = playButtonBounds.contains(mousePos);
            drawStyledButton(app, "PLAY", font, Vector2f(playButtonX, playButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), playHovered, 36);

            instructionsText.setPosition(Vector2f(PADDING, 280));
            app.draw(instructionsText);

            controlsText.setPosition(Vector2f(PADDING, 310));
            app.draw(controlsText);

            hsLabelText.setPosition(Vector2f(PADDING, 450));
            app.draw(hsLabelText);

            hsText.setString(std::to_string(highScore));
            hsText.setPosition(Vector2f(PADDING, 470));
            app.draw(hsText);
        }
        else if (state == PLAYING || state == PAUSED)
        {
            for (int i = 0; i < activePlatforms; i++)
            {
                sPlat.setPosition(Vector2f(plat[i].x, plat[i].y));
                app.draw(sPlat);
            }

            sPers.setPosition(Vector2f(x, y));
            app.draw(sPers);

            scoreTextShadow.setPosition(Vector2f(13, 13));
            scoreTextShadow.setString(std::to_string(currentScore));
            app.draw(scoreTextShadow);

            scoreText.setPosition(Vector2f(10, 10));
            scoreText.setString(std::to_string(currentScore));
            app.draw(scoreText);

            scoreLabelText.setPosition(Vector2f(10, 40));
            app.draw(scoreLabelText);

            if (state == PAUSED)
            {
                app.draw(overlay);

                float pausedX = (WINDOW_WIDTH - pausedText.getGlobalBounds().size.x) / 2;
                pausedText.setPosition(Vector2f(pausedX, 80));
                app.draw(pausedText);

                app.draw(resumeButton);

                float resumeLabelX = (WINDOW_WIDTH - resumeLabel.getGlobalBounds().size.x) / 2;
                resumeLabel.setPosition(Vector2f(resumeLabelX, 280));
                app.draw(resumeLabel);

                float exitButtonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
                float exitButtonY = 330;
                pauseExitButtonBounds = FloatRect(Vector2f(exitButtonX, exitButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
                bool exitHovered = pauseExitButtonBounds.contains(mousePos);
                drawStyledButton(app, "EXIT", font, Vector2f(exitButtonX, exitButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), exitHovered, 32);
            }
        }
        else if (state == GAME_OVER)
        {
            app.draw(pauseOverlay);

            float gameOverX = (WINDOW_WIDTH - gameOverText.getGlobalBounds().size.x) / 2;
            gameOverText.setPosition(Vector2f(gameOverX, 50));
            app.draw(gameOverText);

            float scoreLabelX = (WINDOW_WIDTH - scoreLabel.getGlobalBounds().size.x) / 2;
            scoreLabel.setPosition(Vector2f(scoreLabelX, 130));
            app.draw(scoreLabel);

            finalScoreText.setString(std::to_string(currentScore));
            float scoreX = (WINDOW_WIDTH - finalScoreText.getGlobalBounds().size.x) / 2;
            finalScoreText.setPosition(Vector2f(scoreX, 160));
            app.draw(finalScoreText);

            float sep1X = (WINDOW_WIDTH - separator1.getGlobalBounds().size.x) / 2;
            separator1.setPosition(Vector2f(sep1X, 225));
            app.draw(separator1);

            float hsLabelX = (WINDOW_WIDTH - highScoreLabel.getGlobalBounds().size.x) / 2;
            highScoreLabel.setPosition(Vector2f(hsLabelX, 250));
            app.draw(highScoreLabel);

            highScoreText.setString(std::to_string(highScore));
            float hsX = (WINDOW_WIDTH - highScoreText.getGlobalBounds().size.x) / 2;
            highScoreText.setPosition(Vector2f(hsX, 275));
            app.draw(highScoreText);

            float playAgainButtonX = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
            float playAgainButtonY = 350;
            gameOverPlayAgainButtonBounds = FloatRect(Vector2f(playAgainButtonX, playAgainButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
            bool playAgainHovered = gameOverPlayAgainButtonBounds.contains(mousePos);
            drawStyledButton(app, "PLAY AGAIN", font, Vector2f(playAgainButtonX, playAgainButtonY), Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT), playAgainHovered, 22);

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