#include "RectCollider.h"

RectCollider::RectCollider(const sf::Rect<int>& body) :
	m_body(body) {}

RectCollider::RectCollider(const sf::RectangleShape& bodyShape) :
	m_body(static_cast<sf::Vector2i>(bodyShape.getPosition()), static_cast<sf::Vector2i>(bodyShape.getSize())) {}

RectCollider::RectCollider(const sf::Vector2i& position, const sf::Vector2i& size) :
	m_body(position, size) {}

RectCollider::RectCollider(sf::Vector2i&& position, sf::Vector2i&& size) :
	m_body(position, size) {}

RectCollider::RectCollider(const sf::Vector2f& position, const sf::Vector2f& size) :
	m_body(static_cast<sf::Vector2i>(position), static_cast<sf::Vector2i>(size)) {}

sf::Rect<int> RectCollider::GetRect() const
{
	return m_body;
}

bool RectCollider::DetectMouseCollision(sf::Vector2i&& mousePosition)
{
	return m_body.contains(mousePosition);
}
