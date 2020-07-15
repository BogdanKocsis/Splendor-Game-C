#pragma once
#include "ICard.h"
#include  "IToken.h"

#include <unordered_map>

class ExpansionCard :public ICard
{
public:
	enum class Level {
		Level1 = 1,
		Level2,
		Level3
	};

public:
	// Constr. + Rule of 5
	ExpansionCard(Level level,uint16_t id=0,bool isFaceUp = true);
	ExpansionCard(const ExpansionCard& other);
	ExpansionCard(ExpansionCard&& other);
	ExpansionCard& operator=(const ExpansionCard& other);
	ExpansionCard& operator=(ExpansionCard&& other);
	~ExpansionCard();

	// G&S
	Level GetLevel() const;
	GemType GetRewardType() const;
	GemsMap GetCost() const;

private:
	Level m_level;
	IToken::Type m_rewardType;
	std::unordered_map<IToken::Type, uint16_t> m_cost;

};

