#include "ICard.h"

ICard::ICard(uint16_t id, bool isFaceUp, uint16_t prestigePoints) :
	m_id(id),
	m_isFaceUp(isFaceUp),
	m_prestigePoints(prestigePoints) {}

ICard::~ICard() = default;

ICard& ICard::operator=(ICard && other)
{
	m_isFaceUp = other.m_isFaceUp;
	m_id = other.m_id;
	return *this;
}

ICard& ICard::operator=(const ICard & other)
{
	m_isFaceUp = other.m_isFaceUp;
	m_id = other.m_id;
	return *this;
}

ICard::ICard(ICard && other)
{
	*this = std::move(other);
}

ICard::ICard(const ICard & other)
{
	*this = other;
}

void ICard::SetPrestigePoints(uint16_t prestigePoints)
{
	m_prestigePoints = prestigePoints;
}

uint16_t ICard::GetId() const
{
	return m_id;
}

bool ICard::GetIsFaceUp() const
{
	return m_isFaceUp;
}

void ICard::SetIsFaceUp(bool isFaceUp)
{
	m_isFaceUp = isFaceUp;
}

uint16_t ICard::GetPrestigePoints() const
{
	return m_prestigePoints;
}
