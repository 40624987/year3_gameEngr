//game.h
#pragma once
#include <SFML/Graphics.hpp>
#include "ship.h" // To include Ship, Player, and Invader classes
#include "bullet.h" // To include Bullet class
constexpr uint16_t gameWidth = 800;
constexpr uint16_t gameHeight = 600;
constexpr uint16_t invaders_rows = 5;
constexpr uint16_t invaders_columns = 12;

extern sf::Texture spritesheet;
extern std::vector<Ship*> ships;
extern Player* player;     
extern std::vector<Invader*> invaders;     // Vector holding invader objects

void Load();                               // Function to load resources
void Update(sf::RenderWindow& window);     // Function to update the game state
void Render(sf::RenderWindow& window);     // Function to render the game scene