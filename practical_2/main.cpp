//main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ship.h"
#include "bullet.h"
#include "game.h"





using namespace sf;
using namespace std;
Player* player = nullptr;  // Define player as a global variable
sf::Texture spritesheet;
sf::Sprite invader;
std::vector<Ship*> ships;

// Global variables
Vector2i invaderVelocity;
//const int gameWidth = 800;
//const int gameHeight = 600;
//RectangleShape invader[1];

// Function declarations
void Load();
void Update(RenderWindow& window);
void Render(RenderWindow& window);


void Load() {
    if (!spritesheet.loadFromFile("res/img/invaders_sheet.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }

    // Initialize player
    player = new Player();  // Make sure to initialize the global player pointer
    ships.push_back(player); // Add player to ships vector

    int numRows = 3;  // Number of rows of invaders
    int numCols = 7;  // Number of invaders per row (columns)
    float startX = 100;    // Starting X position for the grid
    float startY = 100;    // Starting Y position for the grid
    float spacingX = 100;   // Horizontal space between invaders
    float spacingY = 80;   // Vertical space between rows

    // Create the player and add it to the ships vector
    Player* player = new Player();
    ships.push_back(player);

    // Nested loop to create invaders in a grid
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            // Calculate the position for each invader
            float xPos = startX + col * spacingX;
            float yPos = startY + row * spacingY;

            // Create a different Invader for each row with a different texture rect
            sf::IntRect invaderRect;

            // Assign different sprite parts based on the row
            if (row == 0) {
                invaderRect = sf::IntRect(0, 0, 32, 32);  // Invader Type 1
            }
            else if (row == 1) {
                invaderRect = sf::IntRect(32, 0, 32, 32); // Invader Type 2
            }
            else if (row == 2) {
                invaderRect = sf::IntRect(64, 0, 32, 32); // Invader Type 3
            }
            // Create a new Invader and add it to the ships vector
            Invader* inv = new Invader(invaderRect, { xPos, yPos });
            ships.push_back(inv);
        }
    }
}



void Update(RenderWindow& window) {
    // Reset clock, recalculate deltaTime (dt)
    static Clock clock;
    float dt = clock.restart().asSeconds();

    // Check for events (e.g., closing the window)
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    // Update all the ships (players and invaders)
    for (auto& s : ships) {
        s->Update(dt);
    }

    // Update the bullets in the bullet pool
    Bullet::Update(dt);
}
void Render(RenderWindow& window) {
    // Render all the ships (player, invaders, etc.)
    for (const auto& s : ships) {
        window.draw(*s);
    }

    // Render bullets (both player and enemy bullets)
    Bullet::Render(window);
}

int main() {
    // Create a window for the game
    sf::RenderWindow window(VideoMode(gameWidth, gameHeight), "Space Invaders");

    // Load game assets (sprites, textures, etc.)
    Load();

    // Game loop
    while (window.isOpen()) {
        window.clear();   // Clear the window for the next frame
        Update(window);   // Update game logic (movement, firing, etc.)
        Render(window);   // Render all game entities
        window.display(); // Display the new frame
    }

    return 0;
}