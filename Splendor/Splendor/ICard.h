#pragma once
#include <cstdint>
#include <unordered_map>

#include "IToken.h"

class ICard
{
public:
	using GemType = IToken::Type;
	using GemsMap = std::unordered_map<GemType, uint16_t>;

public:
	// Constr. + Rule of 5
	ICard(uint16_t id, bool isFaceUp, uint16_t prestigePoints = 0);
	ICard& operator=(ICard&& other);
	ICard& operator=(const ICard& other);
	ICard(ICard&& other);
	ICard(const ICard &other);
	virtual ~ICard();

	// G&S
	uint16_t GetId() const;
	bool GetIsFaceUp() const;
	void SetIsFaceUp(bool isFaceUp);
	uint16_t GetPrestigePoints() const;

protected:
	// G&S
	void SetPrestigePoints(uint16_t prestigePoints);

protected:
	// Logic
	uint16_t m_prestigePoints;
	bool m_isFaceUp;

	// DAO
	uint16_t m_id;
};

