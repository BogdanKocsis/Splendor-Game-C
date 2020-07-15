#include "ExpansionCard.h"
#include "CardDAO.h"

ExpansionCard::ExpansionCard(Level level, uint16_t id, bool isFaceUp) :
	ICard(id, isFaceUp),
	m_level(level)
{
	// Select card from database
	CardDAO::Expansion card;
	switch (level)
	{
	case Level::Level1:
		card = CardDAO::GetL1Expansion(id);
		break;
	case Level::Level2:
		card = CardDAO::GetL2Expansion(id);
		break;
	case Level::Level3:
		card = CardDAO::GetL3Expansion(id);
		break;
	default:
		break;
	}

	// Set-up object's properties
	m_rewardType = card.reward;
	m_prestigePoints = card.prestige;
	m_cost = card.request;
}

ExpansionCard::ExpansionCard(const ExpansionCard& other) :ICard(other.m_id, other.m_isFaceUp)
{
	*this = other;
}

ExpansionCard::ExpansionCard(ExpansionCard&& other): ICard(other.m_id, other.m_isFaceUp)
{
	*this = std::move(other);
}

ExpansionCard& ExpansionCard::operator=(const ExpansionCard& other)
{
	m_level = other.m_level;
	m_cost = other.m_cost;
	m_rewardType = other.m_rewardType;
	
	m_isFaceUp = other.m_isFaceUp;
	m_id = other.m_id;
	m_prestigePoints = other.m_prestigePoints;

	return *this;
}

ExpansionCard& ExpansionCard::operator=(ExpansionCard&& other)
{
	m_level = other.m_level;
	m_cost = other.m_cost;
	m_rewardType = other.m_rewardType;

	m_isFaceUp = other.m_isFaceUp;
	m_id = other.m_id;
	m_prestigePoints = other.m_prestigePoints;

	return *this;
	
}

ExpansionCard::~ExpansionCard()
{
}

ExpansionCard::Level ExpansionCard::GetLevel() const
{
	return m_level;
}

ICard::GemType ExpansionCard::GetRewardType() const
{
	return m_rewardType;
}

ICard::GemsMap ExpansionCard::GetCost() const
{
	return m_cost;
}
