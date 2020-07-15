#pragma once
#include "Randomizer.h"
#include "ExpansionCard.h"
#include "NobleCard.h"
#include "GamePieces.h"

#include <string>
#include <vector>

class NetworkPacket;

template <class cardType, size_t level = NULL>
class Deck
{
public:
	Deck();

	// Logic
	void ShuffleDeck()
	{
		Randomizer rnd;
		rnd.Shuffle(m_cards);
	}

	cardType DrawCard()
	{
		if (m_cards.empty())
			throw std::out_of_range("Can't draw card from empty deck");
		cardType topCard = m_cards.back();
		m_cards.pop_back();
		return topCard;
	}

	void RemoveTopCard()
	{
		if (m_cards.empty())
			throw std::out_of_range("Can't remove card from empty deck");
		m_cards.pop_back();
	}

	void AddCard(cardType&& card)
	{
		m_cards.push_back(std::move(card));
	}

	bool IsEmpty()
	{
		return m_cards.empty();
	}

	size_t GetNumberOfCards() const
	{
		return m_cards.size();
	}

	std::vector<cardType> GetCards() const
	{
		return m_cards;
	}

protected:
	friend NetworkPacket;

	void SetCards(const std::vector<cardType>& cards)
	{
		m_cards = cards;
	}

	void SetCards(std::vector<cardType>&& cards)
	{
		m_cards = std::move(cards);
	}

	void ResetCards()
	{
		m_cards.clear();
	}

private:
	std::vector<cardType> m_cards;

};
