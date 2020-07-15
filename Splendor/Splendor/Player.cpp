#include "Player.h"
#include "NetworkPacket.h"

Player::Player(uint16_t id, std::string&& name, Type type) :
	m_id(id),
	m_name(std::move(name)),
	m_type(type),
	m_prestigePoints(0) {}

std::string Player::GetName() const
{
	return m_name;
}

void Player::SetName(const std::string& name)
{
	m_name = name;
}

Player::Type Player::GetType() const
{
	return m_type;
}

uint16_t Player::GetPrestigePoints() const
{
	return m_prestigePoints;
}

Hand& Player::GetHand()
{
	return m_hand;
}

uint16_t Player::GetId() const
{
	return m_id;
}

void Player::AddPrestigePoints(uint16_t amount)
{
	m_prestigePoints += amount;
}

std::string Player::ConvertToPackage() const
{
	return std::to_string(m_prestigePoints);
}

void Player::ConvertFromPackage(NetworkPacket& networkPacket)
{
	m_prestigePoints = std::stoi(networkPacket.m_playerPrestigePoints);
}
