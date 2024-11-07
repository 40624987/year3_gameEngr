//bullet.cpp
#include "bullet.h"
#include "game.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

// Static array to hold all bullets (maximum 256 bullets)
Bullet Bullet::bullets[256];

// Static pointer to track the next available bullet
unsigned char Bullet::bulletPointer = 0;

Bullet::Bullet() : _mode(false) {
    // Set initial state of all bullets to be off-screen (inactive)
    setPosition(-100.f, -100.f);
    setTexture(spritesheet);
    setOrigin(16.0f, 16.0f);  // Assuming bullet size is 16x16
}

void Bullet::Init() {
    // Initialize all bullets in the pool as inactive (off-screen)
    for (int i = 0; i < 256; ++i) {
        bullets[i].setPosition(-100.f, -100.f);  // Set bullets off-screen
        bullets[i].setTexture(spritesheet);
        bullets[i].setOrigin(8.f, 8.f);  // Assuming bullet size is 16x16
        bullets[i]._mode = false;  // Mode will be set when fired (player or invader)
    }
}

void Bullet::Fire(const sf::Vector2f& pos, const bool mode) {
    // Get the next available bullet from the pool
    Bullet& bullet = bullets[++bulletPointer % 256];  // Wrap around after 256 bullets

    // Set the bullet's properties
    bullet.setPosition(pos);
    bullet._mode = mode;

    // Set the correct bullet sprite depending on the mode
    if (mode) {
        // Enemy bullet (assuming it's the third sprite in row 2, 64x32)
        bullet.setTextureRect(IntRect(64, 32, 16, 16));
    }
    else {
        // Player bullet (assuming it's the second sprite in row 2, 32x32)
        bullet.setTextureRect(IntRect(32, 32, 16, 16));
    }
}

void Bullet::_Update(const float& dt) {
    if (getPosition().y < -32 || getPosition().y > gameHeight + 32) {
        // Bullet is off-screen, do nothing
        return;
    }
    else {
        move(Vector2f(0, dt * 200.0f * (_mode ? 1.0f : -1.0f))); // Move bullet up or down based on mode
        const FloatRect boundingBox = getGlobalBounds();

        // Loop through all ships to check for collisions
        for (auto s : ships) {
            if (!_mode && s == player) {
                // Player bullets don't collide with player
                continue;
            }
            if (_mode && s != player) {
                // Invader bullets don't collide with other invaders
                continue;
            }
            // Check if the bullet intersects with a ship and if the ship hasn't exploded
            if (!s->is_exploded() && s->getGlobalBounds().intersects(boundingBox)) {
               // std::cout << "Bullet hit a ship!" << std::endl;  // Debugging line
                // Explode the ship
                s->Explode();
                // Set bullet off-screen (inactive)
                setPosition(Vector2f(-100, -100));
                return;
            }
        }
    }
}

void Bullet::Update(const float& dt) {
    // Update all active bullets
    for (int i = 0; i < 256; ++i) {
        bullets[i]._Update(dt);
    }
}

void Bullet::Render(sf::RenderWindow& window) {
    // Draw all active bullets
    for (int i = 0; i < 256; ++i) {
        if (bullets[i].getPosition().y > 0.f && bullets[i].getPosition().y < gameHeight) {
            window.draw(bullets[i]);
        }
    }
}
