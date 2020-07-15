#pragma once
#include "Collider.h"

class RectCollider :public Collider
{
public:
	RectCollider(const sf::Rect<int>& body);
	RectCollider(const sf::RectangleShape& bodyShape);
	RectCollider(const sf::Vector2i& position, const sf::Vector2i& size);
	RectCollider(sf::Vector2i&& position, sf::Vector2i&& size);
	RectCollider(const sf::Vector2f& position, const sf::Vector2f& size);

	sf::Rect<int> GetRect() const;
	
	bool DetectMouseCollision(sf::Vector2i&& mousePosition) override;

protected:
	sf::Rect<int> m_body;
};

