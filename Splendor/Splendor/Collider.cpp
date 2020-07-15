#include "Collider.h"

void Collider::HandleEvent(const sf::Event& event)
{
	const bool wasMouseIn = m_isMouseIn;
	switch (event.type)
	{
	case sf::Event::MouseMoved:
		if (DetectMouseCollision(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)))
		{
			m_isMouseIn = true;
			OnMouseOver();
		}
		else
		{
			m_isMouseIn = false;
		}
		break;
	case sf::Event::MouseButtonPressed:
		if (DetectMouseCollision(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				OnMouseLeftClick();
			}
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				OnMouseRightClick();
			}
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (DetectMouseCollision(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				OnMouseLeftRelease();
			}
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				OnMouseRightRelease();
			}
		}
		break;
	default:
		break;
	}

	if (wasMouseIn)
	{
		if (!m_isMouseIn)
		{
			OnMouseLeave();
		}
	}
	else
	{
		if (m_isMouseIn)
		{
			OnMouseEnter();
		}
	}
}

void Collider::OnMouseOver() { /*empty*/ }

void Collider::OnMouseEnter() { /*empty*/ }

void Collider::OnMouseLeave() { /*empty*/ }

void Collider::OnMouseLeftClick() { /*empty*/ }

void Collider::OnMouseRightClick() { /*empty*/ }

void Collider::OnMouseLeftRelease() { /*empty*/ }

void Collider::OnMouseRightRelease() { /*empty*/ }
