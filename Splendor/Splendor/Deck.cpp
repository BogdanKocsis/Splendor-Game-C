#include "Deck.h"

template <>
Deck<NobleCard>::Deck()
{
	// Determine cards count
	const uint16_t cardsCount = GamePieces::s_NobleCardCount;

	// Build deck
	for (uint16_t cardId = 1; cardId <= cardsCount; ++cardId)
	{
		m_cards.emplace_back(cardId, false);
	}
}

template <>
Deck<ExpansionCard, NULL>::Deck()
{
	throw std::invalid_argument("Failed to initialize deck of type <ExpansionCard, NULL>");
}

template <>
Deck<ExpansionCard, 1>::Deck()
{
	// Determine cards count
	const uint16_t cardsCount = GamePieces::s_L1ExpansionCardCount;

	// Build deck
	for (uint16_t cardId = 1; cardId <= cardsCount; ++cardId)
	{
		m_cards.emplace_back(ExpansionCard::Level::Level1, cardId, false);
	}
}

template <>
Deck<ExpansionCard, 2>::Deck()
{
	// Determine cards count
	const uint16_t cardsCount = GamePieces::s_L2ExpansionCardCount;

	// Build deck
	for (uint16_t cardId = 1; cardId <= cardsCount; ++cardId)
	{
		m_cards.emplace_back(ExpansionCard::Level::Level2, cardId, false);
	}
}

template <>
Deck<ExpansionCard, 3>::Deck()
{
	// Determine cards count
	const uint16_t cardsCount = GamePieces::s_L3ExpansionCardCount;

	// Build deck
	for (uint16_t cardId = 1; cardId <= cardsCount; ++cardId)
	{
		m_cards.emplace_back(ExpansionCard::Level::Level3, cardId, false);
	}
}