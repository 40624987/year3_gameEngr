//ship.cpp
#include "ship.h"
#include "game.h"
#include <SFML/Window.hpp>  // For player input (keyboard)

using namespace sf;
using namespace std;

bool Invader::direction = true;
float Invader::speed = 100.f;

// Default constructor for Ship
Ship::Ship() : exploded(false), isFading(false) {}

Ship::Ship(sf::IntRect ir) : Sprite(), exploded(false), isFading(false) {
    _sprite = ir;
    setTexture(spritesheet);
    setTextureRect(_sprite);
}

// Define the ship destructor  
Ship::~Ship() = default;

// function to update ships 
void Ship::Update(const float& dt) {
    if (is_exploded()) {
        HandleFadeOut(dt);  // Handle fade-out effect if exploded
    }
}

// Function to check if the ship is exploded
bool Ship::is_exploded() const {
    return exploded;
}

// Mark the ship as exploded and hide the sprite
void Ship::Explode() {
    exploded = true;
    isFading = true;  // Start fading effect upon explosion

    // Optionally hide the sprite or set an explosion animation
    setTextureRect(IntRect(128, 32, 32, 32));  // Assuming the explosion is at (128, 32) in the spritesheet
    setOrigin(16.f, 16.f);  // Resetting the origin to the center of the sprite
    fadeClock.restart();    // Restart the fade clock when explosion occurs
}

void Ship::HandleFadeOut(const float& dt) {
    if (isFading) {
        sf::Time elapsedTime = fadeClock.getElapsedTime();
        float fadeDuration = 2.0f; // Fade duration in seconds

        // Gradually reduce the alpha value over time
        int alpha = 255 - static_cast<int>(elapsedTime.asSeconds() * 255.0f / fadeDuration);

        if (alpha > 0) {
            setColor(sf::Color(255, 255, 255, alpha));  // Set transparency
        }
        else {
            setColor(sf::Color(255, 255, 255, 0)); // Ensure full transparency at the end
            setPosition(-100.f, -100.f);  // Move off-screen to prevent rendering

            // Mark for removal from the container
            exploded = false;
            isFading = false;  // End the fading effect

            // If you're using a container (like a vector), you might mark it for removal:
            // If using the container, you might need a game state to remove objects safely.
            // For example: container.erase(std::remove(container.begin(), container.end(), this), container.end());
        }
    }
}



// Invader class implementation
Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir) {
    setOrigin(Vector2f(16.f, 16.f));
    setPosition(pos);
}

// Static members to manage movement state and control floating behavior
bool Invader::movingDown = true;   // Direction flag: true for down, false for up
int Invader::stepsMoved = 0;       // Track the number of steps moved in the current direction
const int Invader::maxSteps = 2;   // Number of steps before changing direction (down or up)

void Invader::Update(const float& dt) {
    if (is_exploded()) {
        return;  // If exploded, don't update or fire
    }

    // Move the invader horizontally, based on direction
    move(Vector2f(dt * (direction ? 1.0f : -1.0f) * speed, 0.0f));

    // Check for screen boundaries and move invaders down
    if ((direction && getPosition().x > gameWidth - 16) ||
        (!direction && getPosition().x < 16)) {
        direction = !direction;
        for (auto& s : ships) {
            s->MoveDown();  // Move all invaders down when hitting boundary
        }
    }

    // Randomly fire bullets (simple logic: every few seconds, fire)
    static float firetime = 0.0f;
    firetime -= dt;
    
        if (firetime <= 0 && rand() % 100 == 0) {
            if (!is_exploded()) {
                Bullet::Fire(getPosition(), true);
            }
            firetime = 4.0f + (rand() % 60);
    }
}

void Invader::MoveDown() {
    

    // Move down twice, then up twice, creating a floating effect
    if (movingDown) {
        move(Vector2f(0.0f, 24.0f));  // Move down by 24 pixels (adjust as needed)
        stepsMoved++;

        if (stepsMoved >= maxSteps) {
            movingDown = false;  // Switch to moving up after reaching the max steps
            stepsMoved = 0;      // Reset the step counter
        }
    }
    else {
        move(Vector2f(0.0f, -24.0f)); // Move up by 24 pixels (adjust as needed)
        stepsMoved++;

        if (stepsMoved >= maxSteps) {
            movingDown = true;   // Switch to moving down after reaching the max steps
            stepsMoved = 0;      // Reset the step counter
        }
    }
}


// Player class implementation
Player::Player() : Ship(IntRect(Vector2i(160, 32), Vector2i(32, 32))) {
    setPosition({ gameWidth * .5f, gameHeight - 32.f });
}

void Player::Update(const float& dt) {
    if (is_exploded()) {
        return;  // Don't update player position or fire if exploded
    }

    // Player movement with speed adjustment
    float speed = 200.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && getPosition().x > 16.f) {
        move(-speed * dt, 0.f); // Move left with a speed of 200 units per second
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && getPosition().x < gameWidth - 16.f) {
        move(speed * dt, 0.f); // Move right with a speed of 200 units per second
    }

    // Shoot bullet when spacebar is pressed, but only if not exploded
    static bool isSpacePressed = false;  // Flag to prevent continuous firing
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isSpacePressed && !is_exploded()) {
        Bullet::Fire(getPosition(), false); // Fire player bullet (mode = false)
        isSpacePressed = true;  // Set flag to prevent continuous firing
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        isSpacePressed = false;  // Reset flag when spacebar is released
    }
}

