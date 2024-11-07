//ship.h
#pragma once
#include <SFML/Graphics.hpp>

class Ship : public sf::Sprite {
protected:
    sf::IntRect _sprite;     // Texture rectangle (for sprite cropping)
    bool exploded;           // Flag to track if the ship is exploded

    // Default constructor hidden to prevent direct instantiation
    Ship();

public:
    // Constructor that takes a sprite
    explicit Ship(sf::IntRect ir);

    // Pure virtual destructor -- makes this an abstract class, avoids undefined behavior
    virtual ~Ship() = 0;

    // Update function, virtual so it can be overridden by derived classes
    virtual void Update(const float& dt);

    // Function to check if the ship is exploded
    bool is_exploded() const;

    // Function to mark the ship as exploded
    virtual void Explode();

    // Virtual function to move down, implemented only by Invader
    virtual void MoveDown() {}
};

class Invader : public Ship {
public:
    static bool direction;
    static float speed;

    Invader(sf::IntRect ir, sf::Vector2f pos);  // Constructor for Invader with specific position and texture
    Invader();  // Default constructor for Invader
    void Update(const float& dt) override;  // Update function to control invader movement and firing

    // Move invader down
    void MoveDown() override;
};

class Player : public Ship {
public:
    Player();  // Constructor for Player
    void Update(const float& dt) override;  // Update function to control player movement and firing
};
