#pragma once
#include "Hand.h"

class NetworkPacket;

class Player
{
public:
	enum class Type
	{
		User,
		Computer
	};

public:
	// Constr.
	Player(uint16_t id, std::string&& name, Type type = Type::User);

	// G&S
	std::string GetName() const;
	void SetName(const std::string& name);
	Type GetType() const;
	uint16_t GetPrestigePoints() const;
	Hand& GetHand();
	uint16_t GetId() const;

	// Logic
	void AddPrestigePoints(uint16_t amount);

	// Networking
	std::string ConvertToPackage() const;
	void ConvertFromPackage(NetworkPacket& networkPacket);

private:
	uint16_t m_id;
	std::string m_name;
	Type m_type;
	uint16_t m_prestigePoints;
	Hand m_hand;
};

