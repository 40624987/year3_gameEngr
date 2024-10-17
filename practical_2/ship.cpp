#include "ship.h"
#include "game.h"

using namespace sf;
using namespace std;
bool Invader::direction;
float Invader::speed;

Ship::Ship() {};

Ship::Ship(IntRect ir) : Sprite() {
	_sprite = ir;
	setTexture(spritesheet);
	setTextureRect(_sprite);
};

void Ship::Update(const float& dt) {}

//Define the ship deconstructor. 
//Although we set this to pure virtual, we still have to define it.
Ship::~Ship() = default;

Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir) {
	setOrigin(Vector2f(16.f, 16.f));;
	setPosition(pos);
}

void Invader::Update(const float& dt) {
	Ship::Update(dt);
	move(Vector2f(dt * (direction ? 1.0f : -1.0f) * speed, 0.0f));

	if ((direction && getPosition().x > gameWidth - 16) ||
		(!direction && getPosition().x < 16)) {
		direction = !direction;
		for (auto& s : ships) {
			s->MoveDown();
		}
	}
}

void Invader::MoveDown() {
	move(Vector2f(0.0f, 24.0f));
}

Player::Player() : Ship(IntRect(Vector2i(160, 32), Vector2i(32, 32))) {
	setPosition({ gameWidth * .5f, gameHeight - 32.f });
}

void Player::Update(const float& dt) {
	Ship::Update(dt);

	// Move left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && getPosition().x > 16.f) {
		move(-200.f * dt, 0.f); // Move left with a speed of 200 units per second
	}

	// Move right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && getPosition().x < gameWidth - 16.f) {
		move(200.f * dt, 0.f); // Move right with a speed of 200 units per second
	}
}
