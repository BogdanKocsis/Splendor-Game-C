#pragma once
#include "UIText.h"

#include <SFML/Graphics.hpp>

class UISelector : sf::Rect<float>
{
public:
	UISelector(const std::string& title, const sf::Vector2f& position = sf::Vector2f(0, 0), const sf::Vector2f& size = sf::Vector2f(100, 100), bool initialState = false);

	// Selector Name
	std::string GetName();

	// State Manipulators
	bool GetState() const;
	virtual void SetState(bool state);
	virtual void ChangeState() = 0;
	bool IsChecked() const;

protected:
	UIText m_title;
	bool m_isChecked;

};

