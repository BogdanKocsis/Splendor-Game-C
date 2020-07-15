#pragma once
#include "RectCollider.h"
#include "UIText.h"

#include <SFML/Graphics.hpp>

class UIButton :public RectCollider, public sf::Drawable
{
public:
	enum class State
	{
		None,
		Hover,
		Press,
		Release
	};
	class Design
	{
	public:
		struct Body
		{
			sf::Color fillColor = sf::Color::Red;
			sf::Color outlineColor = sf::Color::White;
			float outlineThickness = 3.0f;
		};
		struct Text
		{
			std::string text = "Button Text";
			UIText::AvailableFonts font = UIText::AvailableFonts::DosisLight;
			uint16_t fontSize = 30;
			sf::Color fontFill = sf::Color::White;
			sf::Color fontOutline = sf::Color::Black;
			float outlineThickness = 1.5f;
		};
	public:
		Design() = default;
		Design(const Body& bodyDesign, const Text& textDesign);
		Design(Body&& bodyDesign, Text&& textDesign);

		void SetBodyDesign(const Body& bodyDesign);
		void SetTextDesign(const Text& textDesign);

	private:
		friend UIButton;
		Body m_bodyDesign;
		Text m_textDesign;
	};
	struct DefaultDesign
	{
		static const Design NoneDesign;
		static const Design HoverDesign;
		static const Design PressDesign;
	};

public:
	// Constr.
	UIButton(const sf::RectangleShape& body, const Design& none = DefaultDesign::NoneDesign, const Design& hover = DefaultDesign::HoverDesign, const Design& press = DefaultDesign::PressDesign, State initialState = State::None);

	// G&S
	State GetState() const;
	void SwitchState(State newState);
	Design GetDesign() const;
	void ChangeText(const std::string& newText);

	// Events
	void OnMouseEnter() override;
	void OnMouseLeave() override;
	void OnMouseLeftClick() override;
	void OnMouseLeftRelease() override;

	// Graphics
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const override;

private:
	void UpdateDesign();

private:
	// Components
	sf::RectangleShape m_body;
	UIText m_text;
	// Custom Design
	Design m_noneDesign, m_hoverDesign, m_pressDesign;
	Design* m_currentDesign;
	// Functionality
	State m_currentState;
};