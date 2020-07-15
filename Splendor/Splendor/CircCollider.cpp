#include "CircCollider.h"

#include <iostream>

CircCollider::CircCollider(const sf::CircleShape& bodyShape) :m_body(bodyShape) {}

CircCollider::CircCollider(const sf::Vector2f& position, float radius)
{
	m_body.setPosition(position);
	m_body.setRadius(radius);
}

CircCollider::CircCollider(sf::Vector2f&& position, float radius)
{
	m_body.setPosition(std::move(position));
	m_body.setRadius(radius);
}

bool CircCollider::DetectMouseCollision(sf::Vector2i&& mousePosition)
{
	const auto bodyPosition = m_body.getPosition();
	const float distance = sqrt((mousePosition.x - bodyPosition.x) * (mousePosition.x - bodyPosition.x)
		+ (mousePosition.y - bodyPosition.y) * (mousePosition.y - bodyPosition.y));
	return distance < m_body.getRadius();
}

sf::CircleShape& CircCollider::ColliderBody()
{
	return m_body;
}
