#pragma once
#include "SFML\Graphics.hpp"


class Entity
{
public:
	Entity();
	Entity(const sf::Color& color, float left, float top, float width, float height, float vx, float vy);
	~Entity();

	void update(float dt);

public:
	float left, top, width, height;
	float vx, vy;
	sf::Color color;

};

