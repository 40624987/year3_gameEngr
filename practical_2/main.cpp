#include <SFML/Graphics.hpp>
#include <iostream>
#include "ship.h"


using namespace sf;
using namespace std;
sf::Texture spritesheet;
sf::Sprite invader;
std::vector<Ship*> ships;

// Global variables
Vector2i invaderVelocity;
const int gameWidth = 800;
const int gameHeight = 600;
//RectangleShape invader[1];

// Function declarations
void Load();
void Update(RenderWindow& window);
void Render(RenderWindow& window);


void Load() {
    if (!spritesheet.loadFromFile("res/img/invaders_sheet.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }

    int numRows = 3;  // Number of rows of invaders
    int numCols = 5;  // Number of invaders per row (columns)
    float startX = 100;    // Starting X position for the grid
    float startY = 100;    // Starting Y position for the grid
    float spacingX = 100;   // Horizontal space between invaders
    float spacingY = 80;   // Vertical space between rows

    // Nested loop to create invaders in a grid
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            // Calculate the position for each invader
            float xPos = startX + col * spacingX;
            float yPos = startY + row * spacingY;

            // Create a new Invader and add it to the ships vector
            Invader* inv = new Invader(sf::IntRect(Vector2i(0, 0), Vector2i(32, 32)), { xPos, yPos });
            ships.push_back(inv);
        }
    }
}



void Update(RenderWindow& window) {
    // Reset clock, recalculate deltatime
    static Clock clock;
    float dt = clock.restart().asSeconds();

    // Check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }
    for (auto& s : ships) {
        s->Update(dt);
    };
}
void Render(RenderWindow& window) {
    window.draw(invader);
    for (const auto s : ships) {
        window.draw(*s);
    }
}

int main(){
  RenderWindow window(VideoMode(gameWidth, gameHeight), "Space Invaders");
  Load();

  while (window.isOpen()) {
      window.clear();
      Update(window);
      Render(window);
      window.display();
  }
  return 0;
}