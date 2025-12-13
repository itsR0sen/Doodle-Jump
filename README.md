# Doodle Jump

## Author

**Mosharof Rosen**  
Department of Software Engineering  
Shahjalal University of Science and Technology (SUST)

---

## Overview

Doodle Jump is a classic arcade-style platformer game built with C++ and SFML (Simple and Fast Multimedia Library). The objective is simple yet challenging: jump as high as possible by landing on platforms while avoiding falling off the bottom of the screen.

## Game Mechanics

### Objective

-   Jump on platforms to ascend as high as possible
-   Gain points for every unit of height climbed
-   Avoid falling off the bottom of the screen

### Controls

-   **Left Arrow Key**: Move character left
-   **Right Arrow Key**: Move character right
-   **Space Bar**: Pause/Resume the game
-   **ESC**: Return to menu from pause or game over

### Gameplay Features

-   **Dynamic Platform Scrolling**: As you climb higher, the camera follows and platforms scroll
-   **Platform Recycling**: Platforms that go off-screen are recycled at the top with new random positions
-   **Gravity Physics**: Realistic gravity and jump mechanics
-   **Score Tracking**: Real-time score display based on height climbed
-   **High Score Persistence**: Your best score is saved to `highscore.txt`
-   **Pause Functionality**: Pause the game at any time with visual overlay
-   **Game Over Screen**: Shows your current score and best score

## Technical Details

### Requirements

-   **Language**: C++20
-   **Library**: SFML 3.0+ (Simple and Fast Multimedia Library)
-   **Platform**: Windows (Visual Studio)

### Build Instructions

1. Ensure you have SFML 3.0 or later installed
2. Open the project in Visual Studio
3. Build the solution (Ctrl+Shift+B)
4. Run the executable

### Project Structure

```
Doodle_Jump/
├── images/                  # Game assets directory
│   ├── background.png       # Game background image
│   ├── platform.png         # Platform sprite
│   ├── doodle.png           # Player character sprite
│   ├── resume.png           # Resume button image
│   └── font.otf             # UI font file
├── highscore.txt            # Auto-generated high score file
└── README.md                # This file
```

## Game Constants

-   **Window Size**: 400x533 pixels
-   **Platform Count**: 10 platforms
-   **Player Size**: 50x70 pixels
-   **Platform Size**: 68x14 pixels
-   **Gravity**: 0.2 pixels/frame²
-   **Jump Power**: 11.0 pixels/frame
-   **Movement Speed**: 4 pixels/frame
-   **Camera Threshold**: 200 pixels

## UI/UX Features

### Main Menu

-   Attractive title with colorful styling
-   Play button with hover effects
-   Instructions display
-   High score display

### Game Screen

-   Real-time score display with shadow effect
-   Platform and player rendering
-   Smooth collision detection

### Pause Menu

-   Semi-transparent overlay
-   Pause status indicator
-   Resume and Exit buttons
-   Visual feedback on button hover

### Game Over Screen

-   Current score display
-   Best score comparison
-   "Play Again" button to restart
-   Exit button to return to menu

## Color Scheme

-   **Golden Hover**: RGB(255, 200, 87)
-   **Bright Accent**: RGB(0, 255, 150) - Bright green/cyan
-   **Light Blue Text**: RGB(200, 220, 255)
-   **Button Background**: RGB(40, 50, 80) - Dark blue
-   **Dark Background**: RGB(20, 20, 40)

## Performance Optimizations

-   **Pre-allocated UI Objects**: All Text and Shape objects are created once during initialization and reused each frame to minimize memory allocation
-   **Fixed Frame Rate**: Game runs at 60 FPS for smooth, consistent gameplay
-   **Efficient Collision Detection**: Simple but effective box collision for platform interaction
-   **Memory-Efficient**: Constant memory usage throughout gameplay with no leaks

## File I/O

The game automatically manages high score persistence:

-   **Save**: High score is saved to `highscore.txt` when beaten
-   **Load**: High score is loaded on startup
-   Uses standard C++ file streams (`std::ifstream`, `std::ofstream`)

## How to Play

1. **Start the Game**: Click the PLAY button on the main menu
2. **Jump on Platforms**: Use the arrow keys to move left and right
3. **Climb Higher**: Land on platforms to jump higher and gain points
4. **Avoid Falling**: Don't fall off the bottom of the screen
5. **Pause Anytime**: Press SPACE to pause and resume
6. **Game Over**: You lose when you fall below the screen
7. **High Score**: Beat your previous best score!

## Game States

-   **MENU**: Main menu screen
-   **PLAYING**: Active gameplay
-   **PAUSED**: Game paused with overlay
-   **GAME_OVER**: Game over screen with score display

## Credits

Developed using SFML for graphics and event handling.

---

## Feedback & Support

For issues, feature requests, or improvements, please contact the development team at the Department of Software Engineering, SUST.

---

**Version**: 1.0  
**Last Updated**: 2024
