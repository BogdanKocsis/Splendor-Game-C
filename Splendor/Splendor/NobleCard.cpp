#include "NobleCard.h"
#include "CardDAO.h"



NobleCard::NobleCard(uint16_t id, bool isFaceUp) :
	ICard(id, isFaceUp)
{
	const CardDAO::Noble card = CardDAO::GetNoble(id);
	m_prestigePoints = card.prestige;
	m_requirements = card.request;
	m_name = card.name;
}

ICard::GemsMap NobleCard::GetRequirements() const
{
	return m_requirements;
}

std::string NobleCard::GetName() const
{
	return m_name;
}

NobleCard::NobleCard(const NobleCard& other) :ICard(other.m_id, other.m_isFaceUp)
{
	*this = other;
}


NobleCard::NobleCard(NobleCard&& other) : ICard(other.m_id, other.m_isFaceUp)
{
	*this = std::move(other);
}

NobleCard& NobleCard::operator=(const NobleCard& other)
{
	m_name = other.m_name;
	m_requirements = other.m_requirements;
	m_isFaceUp = other.m_isFaceUp;
	m_id = other.m_id;
	m_prestigePoints = other.m_prestigePoints;

	return *this;
}

NobleCard& NobleCard::operator=(NobleCard&& other)
{
	m_name = other.m_name;
	m_requirements = other.m_requirements;
	m_isFaceUp = other.m_isFaceUp;
	m_id = other.m_id;
	m_prestigePoints = other.m_prestigePoints;

	new(&other) NobleCard;

	return *this;

}

