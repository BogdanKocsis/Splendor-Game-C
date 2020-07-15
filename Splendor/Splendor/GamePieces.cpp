#include "GamePieces.h"

uint16_t GamePieces::GetGemTokenCount()
{
	return s_uGemTokenCount;
}

uint16_t GamePieces::GetNobleCardCount()
{
	return s_uNobleCardCount;
}

void GamePieces::SetGemTokenCount(uint16_t newGemTokenCount)
{
	s_uGemTokenCount = newGemTokenCount;
}

void GamePieces::SetNobleCardCount(uint16_t newNobleCardCount)
{
	s_uNobleCardCount = newNobleCardCount;
}
