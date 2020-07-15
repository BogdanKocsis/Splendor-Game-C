#include "PregameSetup.h"

PregameSetup::PregameSetup(uint16_t playerCount, GameMode gameMode, bool withTimer, bool withAI) :
	m_playerCount(playerCount),
	m_gameMode(gameMode),
	m_withTimer(withTimer),
	m_withAI(withAI)
{
	if (playerCount < 2 || playerCount > 4)
		throw "ERROR: Wrong playerCount (Value must be between 2-4)";

	GamePieces::SetNobleCardCount(playerCount + 1);
	switch (playerCount)
	{
	case 2:
		GamePieces::SetGemTokenCount(GamePieces::s_GemTokenCount - 3);
		break;
	case 3:
		GamePieces::SetGemTokenCount(GamePieces::s_GemTokenCount - 2);
		break;
	default:
		GamePieces::SetGemTokenCount(GamePieces::s_GemTokenCount);
	}
}

uint16_t PregameSetup::GetPlayerCount() const
{
	return m_playerCount;
}

PregameSetup::GameMode PregameSetup::GetGameMode() const
{
	return m_gameMode;
}

bool PregameSetup::IsTimer() const
{
	return m_withTimer;
}

bool PregameSetup::IsAI() const
{
	return m_withAI;
}
