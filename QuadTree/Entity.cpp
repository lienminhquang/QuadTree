#include "Entity.h"



Entity::Entity():
    left(0),
    top(0),
    width(0),
    height(0),
    vx(0),
    vy(0)
{
}


Entity::Entity(const sf::Color & color, float left, float top, float width, float height, float vx, float vy):
	color(color),
	left(left),
	top(top),
	width(width),
	height(height),
	vx(vx),
	vy(vy)
{
}

Entity::~Entity()
{
}

void Entity::update(float dt)
{
	left += dt * vx;
	top += dt*vy;

	if (left < 0 || left + width > 800)
		vx = -vx;
	if (top < 0 || top + height > 800)
		vy = -vy;
}
