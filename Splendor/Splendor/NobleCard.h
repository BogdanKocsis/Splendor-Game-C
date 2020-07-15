#pragma once
#include "ICard.h"

class NobleCard : public ICard
{
public:
	// Constr. + Rule of 5
	NobleCard(uint16_t id = 0, bool isFaceUp = true);
	NobleCard(const NobleCard& other);
	NobleCard(NobleCard&& other);
	~NobleCard() = default;
	NobleCard& operator =(const NobleCard& other);
	NobleCard& operator =(NobleCard&& other);


	// G&S
	GemsMap GetRequirements() const;
	std::string GetName() const;


private:
	std::string m_name;
	GemsMap m_requirements;
};

