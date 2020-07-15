#pragma once
#include "Collider.h"

class CircCollider :public Collider
{
public:
	CircCollider(const sf::CircleShape& bodyShape);
	CircCollider(const sf::Vector2f& position, float radius);
	CircCollider(sf::Vector2f&& position, float radius);

	bool DetectMouseCollision(sf::Vector2i&& mousePosition) override;

protected:
	sf::CircleShape& ColliderBody();

private:
	sf::CircleShape m_body;
};

