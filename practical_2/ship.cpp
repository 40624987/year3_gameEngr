//ship.cpp
#include "ship.h"
#include "game.h"
#include <SFML/Window.hpp>  // For player input (keyboard)

using namespace sf;
using namespace std;

bool Invader::direction = true;
float Invader::speed = 100.f;

// Default constructor for Ship, made protected to prevent direct instantiation
Ship::Ship() : exploded(false) {}

Ship::Ship(sf::IntRect ir) : Sprite(), exploded(false) {
    _sprite = ir;
    setTexture(spritesheet);
    setTextureRect(_sprite);
}

// Define the ship destructor
Ship::~Ship() = default;

// Virtual function to update ships (empty for base class)
void Ship::Update(const float& dt) {}

// Function to check if the ship is exploded
bool Ship::is_exploded() const {
    return exploded;
}

// Mark the ship as exploded and hide the sprite
void Ship::Explode() {
    exploded = true;

    // Optionally hide the sprite or set an explosion animation
    setTextureRect(IntRect(96, 0, 32, 32));  // Assuming the explosion is at (96, 0) in the spritesheet
    setOrigin(16.f, 16.f);  // Resetting the origin to the center of the sprite
}

// Invader class implementation
Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir) {
    setOrigin(Vector2f(16.f, 16.f));
    setPosition(pos);
}

void Invader::Update(const float& dt) {
    Ship::Update(dt);

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
    static float fireCooldown = 0.f;  // Simple cooldown for firing bullets
    fireCooldown += dt;  // Increment the timer by the deltaTime

    if (fireCooldown >= 2.f) {
        fireCooldown = 0.f;  // Reset the cooldown timer
        Bullet::Fire(getPosition(), true);  // Fire invader bullet (mode = true)
    }
}

void Invader::MoveDown() {
    move(Vector2f(0.0f, 24.0f));  // Move down when touching screen edges
}

// Player class implementation
Player::Player() : Ship(IntRect(Vector2i(160, 32), Vector2i(32, 32))) {
    setPosition({ gameWidth * .5f, gameHeight - 32.f });
}

void Player::Update(const float& dt) {
    Ship::Update(dt);

    // Player movement with speed adjustment
    float speed = 200.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && getPosition().x > 16.f) {
        move(-speed * dt, 0.f); // Move left with a speed of 200 units per second
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && getPosition().x < gameWidth - 16.f) {
        move(speed * dt, 0.f); // Move right with a speed of 200 units per second
    }

    // Shoot bullet when spacebar is pressed
    static bool isSpacePressed = false;  // Flag to prevent continuous firing
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isSpacePressed) {
        Bullet::Fire(getPosition(), false); // Fire player bullet (mode = false)
        isSpacePressed = true;  // Set flag to prevent continuous firing
    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        isSpacePressed = false;  // Reset flag when spacebar is released
    }
}

