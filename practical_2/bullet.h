//bullet.h
#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::Sprite {
public:
	// Updates all bullets in the pool
	static void Update(const float& dt);

	// Renders all active bullets
	static void Render(sf::RenderWindow& window);

	// Fires a bullet from the pool (sets position and mode)
	static void Fire(const sf::Vector2f& pos, const bool mode);

	// Initializes all bullets (sets them to an inactive state)
	static void Init();

	~Bullet() = default;

protected:
	// A static array of bullets in the pool
	static Bullet bullets[256];

	// A static pointer to keep track of the next available bullet
	static unsigned char bulletPointer;

	// Updates a single bullet's movement
	void _Update(const float& dt);

	// Private constructor for bullet pool
	Bullet();

	// Mode: false = player bullet, true = enemy bullet
	bool _mode;
};
