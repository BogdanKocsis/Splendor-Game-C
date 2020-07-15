#pragma once
#include "UIText.h"
#include "UIButton.h"
#include "UIColors.h"

#include <unordered_map>

class UIMainMenuSession : public sf::Drawable
{
private:
	enum class Buttons
	{
		NewGame,
		Tutorial,
		Settings,
		Exit,
		Test,
		ExitYes,
		ExitNo,
		Leaderboard
	};
	
public:
	enum class Events
	{
		None,
		NewGame,
		Tutorial,
		Settings,
		Exit,
		Test,
		Leaderboard
	};
	
public:
	UIMainMenuSession(const sf::Vector2u& windowSize);
	~UIMainMenuSession();

	// Events
	void PassEvent(const sf::Event& event);
	Events GetEvent();

	// Graphics
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const override;

private:
	// Background
	static inline const std::string s_backgroundTexture = "../external/Resources/Images/Backgrounds/Splendor_background.png";
	// Button Designs
	static inline const UIButton::Design s_buttonNoneDesign = {
		UIButton::Design::Body
		{
		sf::Color(0, 0, 0,0),
		UIColors::GoldYellow,
		4.0f
		},
		UIButton::Design::Text
		{
		"Button",
		UIText::AvailableFonts::DosisBold,
		30,
		UIColors::GoldYellow,
		sf::Color(0,0,0),
		0.0f
		}
	};
	static inline const UIButton::Design s_buttonHoverDesign = {
		UIButton::Design::Body
		{
		UIColors::GoldYellow,
		sf::Color(255,255,255),
		2.0f
		},
		UIButton::Design::Text
		{
		"Button",
		UIText::AvailableFonts::DosisBold,
		30,
		sf::Color(255,255,255),
		sf::Color(255,255,255),
		0.0f
		}
	};
	static inline const UIButton::Design s_buttonPressDesign = {
		UIButton::Design::Body
		{
		UIColors::DarkYellow,
		sf::Color(0,0,0),
		0.0f
		},
		UIButton::Design::Text
		{
		"Button",
		UIText::AvailableFonts::DosisBold,
		30,
		sf::Color(200,200,200),
		sf::Color(255,255,255),
		0.0f
		}
	};

private:
	sf::Texture m_backgroundTexture;
	sf::Sprite m_background;
	sf::RectangleShape m_mainMenu;
	sf::RectangleShape m_exitMenu;
	UIText* m_mainMenuText;
	UIText* m_exitMenuText;
	std::unordered_map<Buttons, UIButton*> m_buttons;
	std::vector<sf::Drawable*> m_mainMenuDrawableVector;
	std::vector<sf::Drawable*> m_exitMenuDrawableVector;
	std::vector<Collider*> m_mainMenuColliderVector;
	std::vector<Collider*> m_exitMenuColliderVector;
	bool m_exitMenuTriggered;
};

