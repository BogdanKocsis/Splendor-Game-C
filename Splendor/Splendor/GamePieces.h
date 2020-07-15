#pragma once
#include <cstdint>

class GamePieces
{
public:
	GamePieces() = delete;

	// Getters
	static uint16_t GetGemTokenCount();
	static uint16_t GetNobleCardCount();

	// Setters
	static void SetGemTokenCount(uint16_t newGemTokenCount);
	static void SetNobleCardCount(uint16_t newNobleCardCount);

public:
	// Total Pieces
	static inline const uint16_t s_TokenCount = 40;
	static inline const uint16_t s_GemTokenCount = 7;
	static inline const uint16_t s_GoldTokenCount = 5;
	static inline const uint16_t s_HeldTokensLimit = 10;

	static inline const uint16_t s_NobleCardCount = 10;

	static inline const uint16_t s_ExpansionCardCount = 90;
	static inline const uint16_t s_L1ExpansionCardCount = 40;
	static inline const uint16_t s_L2ExpansionCardCount = 30;
	static inline const uint16_t s_L3ExpansionCardCount = 20;

	// Win Condition
	static inline const uint16_t s_winingPrestigePoints = 15;

private:
	// Updatable Pieces
	static inline uint16_t s_uGemTokenCount;
	static inline uint16_t s_uNobleCardCount;

};