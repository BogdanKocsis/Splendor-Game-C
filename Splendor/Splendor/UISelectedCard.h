#pragma once
#include "UICard.h"
#include "UIText.h"
#include "UIColors.h"

class UISelectedCard
{
public:
	enum class TextType
	{
		None,
		Half,
		Full,
		InHand
	};

public:
	UISelectedCard() = delete;

	static void Set(sf::Drawable* card);
	static void UpdateText(const sf::Vector2f& position);
	static void DisplayText(TextType displayText);
	static std::pair<sf::Drawable*, sf::Drawable*> Get();

private:
	inline static sf::Drawable* s_selectedCard = nullptr;
	inline static UIText s_textFull = UIText(sf::Vector2f(), UIText::TextAlign::classic, UIText::AvailableFonts::LatoBlack, "LEFT CLICK TO BUY\nRIGHT CLICK TO HOLD", 40, UIColors::GoldYellow, UIColors::OpaqueBlack, 3);
	inline static UIText s_textHalf = UIText(sf::Vector2f(), UIText::TextAlign::classic, UIText::AvailableFonts::LatoBlack, "RIGHT CLICK TO HOLD", 40, UIColors::GoldYellow, UIColors::OpaqueBlack, 3);
	inline static UIText s_textInHand = UIText(sf::Vector2f(), UIText::TextAlign::classic, UIText::AvailableFonts::LatoBlack, "LEFT CLICK TO BUY", 40, UIColors::GoldYellow, UIColors::OpaqueBlack, 3);

	inline static TextType s_displayText = TextType::None;
	const inline static uint16_t s_textDistance = 20;// px
};

