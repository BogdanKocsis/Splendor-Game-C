#pragma once
#include <cstdint>

#include "GamePieces.h"
/*
 Separate the development deck into 3 piles; each pile represents its level by the number of circles along the bottom of the card. Place the 3 decks into the middle of the play area and draw the top 4 cards from each deck, placing each face up in the play area horizontally to the deck they were drawn from.

Shuffle the noble tiles and place up to 5 of them face up in the play area. The number of nobles is determined by the number of players plus one.

Special notes: If playing with 2 players, then reduce all token piles to 4 by removing 3 from each, but leave the gold tokens at 5.

If playing with 3 players, then reduce all token piles to 5 by removing 2 from each, but leave the gold tokens at 5.
 */

class PregameSetup
{
public:
	enum class GameMode
	{
		Offline,
		Client,
		Server
	};

public:
	PregameSetup(uint16_t playerCount, GameMode gameMode, bool withTimer, bool withAI);

	uint16_t GetPlayerCount() const;
	GameMode GetGameMode() const;
	bool IsTimer() const;
	bool IsAI() const;

private:
	uint16_t m_playerCount;
	GameMode m_gameMode;
	bool m_withTimer;
	bool m_withAI;
};

