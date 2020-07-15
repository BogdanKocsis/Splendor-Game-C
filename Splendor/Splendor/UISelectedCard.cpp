#include "UISelectedCard.h"

void UISelectedCard::Set(sf::Drawable* card)
{
	s_selectedCard = card;
}

void UISelectedCard::UpdateText(const sf::Vector2f& position)
{
	// Full Text
	const auto textBounds1 = s_textFull.getLocalBounds();
	s_textFull.setPosition(position - sf::Vector2f(textBounds1.width + s_textDistance, 0));

	// Half Text
	const auto textBounds2 = s_textHalf.getLocalBounds();
	s_textHalf.setPosition(position - sf::Vector2f(textBounds2.width + s_textDistance, 0));

	// InHand Text
	s_textInHand.setPosition(position + sf::Vector2f(s_textDistance, 0));
}

void UISelectedCard::DisplayText(TextType displayText)
{
	s_displayText = displayText;
}

std::pair<sf::Drawable*, sf::Drawable*> UISelectedCard::Get()
{
	switch (s_displayText)
	{
	case TextType::None:
		return std::make_pair(s_selectedCard, nullptr);
	case TextType::Half:
		return std::make_pair(s_selectedCard, dynamic_cast<sf::Drawable*>(&s_textHalf));
	case TextType::Full:
		return std::make_pair(s_selectedCard, dynamic_cast<sf::Drawable*>(&s_textFull));
	case TextType::InHand:
		return std::make_pair(s_selectedCard, dynamic_cast<sf::Drawable*>(&s_textInHand));
	default:
		return std::make_pair(s_selectedCard, nullptr);
	}
}
