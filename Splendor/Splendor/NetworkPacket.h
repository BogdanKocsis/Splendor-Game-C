#pragma once
#include <SFML/Network/Packet.hpp>
#include <sstream>

#include "Board.h"
#include "Hand.h"

class NetworkPacket
{
public:
	static inline std::string s_delimiter = "-";

public:
	// Data Manipulators
	void SetHandData(const std::tuple<std::string, std::string, std::string, std::string>& handData);
	void SetBoardData(const std::tuple<std::string, std::string, std::string, std::string, std::string>& boardData);
	void SetCardDrawnFromDeck(Board::LastDrawn cardDrawn);
	void SetDecksData(const std::tuple < std::string, std::string, std::string, std::string>& decksData);
	void SetPlayerData(const std::string& prestigePoints);
	void ClearData();

	// Package Tokenizers
	template <class slotType, size_t size, ExpansionCard::Level level = ExpansionCard::Level::Level1>
	static void TokenizePackage(std::array<std::optional<slotType>, size>& toSlots, std::string&& fromString)
	{
		if (fromString.empty()) return;

		for (auto& slot : toSlots)
		{
			slot.reset();
		}
		size_t position = 0;
		size_t iterator = 0;
		while ((position = fromString.find(s_delimiter)) != std::string::npos)
		{
			const auto& token = fromString.substr(0, position);
			toSlots[iterator].emplace(std::stoi(token));
			fromString.erase(0, position + s_delimiter.length());
			++iterator;
		}
	}

	template <ExpansionCard::Level level>
	static void TokenizePackage(std::array<std::optional<ExpansionCard>, 4>& toSlots, std::string&& fromString)
	{
		if (fromString.empty()) return;

		for (auto& slot : toSlots)
		{
			slot.reset();
		}
		size_t position = 0;
		size_t iterator = 0;
		while ((position = fromString.find(s_delimiter)) != std::string::npos)
		{
			const auto& token = fromString.substr(0, position);
			toSlots[iterator].emplace(level, std::stoi(token));
			fromString.erase(0, position + s_delimiter.length());
			++iterator;
		}
	}

	template <>
	static void TokenizePackage(std::array<std::optional<ExpansionCard>, 3>& toSlots, std::string&& fromString)
	{
		for (auto& slot : toSlots)
		{
			slot.reset();
		}
		size_t position = 0;
		size_t iterator = 0;
		while ((position = fromString.find(s_delimiter)) != std::string::npos)
		{
			const auto token = fromString.substr(0, position);
			const auto& tokenLevel = token.substr(0, 1);
			const auto& tokenId = token.substr(1, token.size() - 1);
			toSlots[iterator].emplace(static_cast<ExpansionCard::Level>(std::stoi(tokenLevel)), std::stoi(tokenId));
			fromString.erase(0, position + s_delimiter.length());
			++iterator;
		}
	}

	static void TokenizePackage(std::unordered_map<IToken::Type, uint16_t>& toGemsMap, std::string&& fromString)
	{
		if (fromString.empty()) return;

		std::stringstream tokensStream(fromString);
		char tokenType;
		uint16_t tokenCount;
		while (tokensStream >> tokenType >> tokenCount)
		{
			switch (tokenType)
			{
			case 'E':
				toGemsMap[IToken::Type::GreenEmerald] = tokenCount;
				break;
			case 'S':
				toGemsMap[IToken::Type::BlueSapphire] = tokenCount;
				break;
			case 'D':
				toGemsMap[IToken::Type::WhiteDiamond] = tokenCount;
				break;
			case 'O':
				toGemsMap[IToken::Type::BlackOnyx] = tokenCount;
				break;
			case 'R':
				toGemsMap[IToken::Type::RedRuby] = tokenCount;
				break;
			case 'G':
				toGemsMap[IToken::Type::Gold] = tokenCount;
				break;
			default:
				throw std::invalid_argument("Corrupted token string");
			}
		}
	}

	template <class cardType, size_t level>
	static void TokenizePackage(Deck<cardType, level>& toDeck, std::string&& fromString)
	{
		if (fromString.empty()) return;

		std::vector<cardType> cards;

		size_t position = 0;
		size_t iterator = 0;
		while ((position = fromString.find(s_delimiter)) != std::string::npos)
		{
			const auto& token = fromString.substr(0, position);
			cards.emplace_back(std::stoi(token));
			fromString.erase(0, position + s_delimiter.length());
			++iterator;
		}

		toDeck.SetCards(std::move(cards));
	}

	template <size_t level>
	static void TokenizePackage(Deck<ExpansionCard, level>& toDeck, std::string&& fromString)
	{
		if (fromString.empty()) return;

		std::vector<ExpansionCard> cards;

		size_t position = 0;
		while ((position = fromString.find(s_delimiter)) != std::string::npos)
		{
			const auto& token = fromString.substr(0, position);
			cards.emplace_back(static_cast<ExpansionCard::Level>(level), std::stoi(token));
			fromString.erase(0, position + s_delimiter.length());
		}

		toDeck.SetCards(std::move(cards));
	}

	// Inspection ostream operator
	friend std::ostream& operator <<(std::ostream& out, const NetworkPacket& packet);

public:
	// Hand
	std::string m_handResources;
	std::string m_handTokens;
	std::string m_handExpansions;
	std::string m_handNoble;

	// Board
	std::string m_boardTokensString;
	std::string m_boardNobleSlotsString;
	std::string m_boardExpansionL1SlotsString;
	std::string m_boardExpansionL2SlotsString;
	std::string m_boardExpansionL3SlotsString;
	std::string m_cardDrawnFromDeck;	//0 = no card drawn, 1 = from L1 deck, 2 = ..L2, 3 = ..L3

	// Decks
	std::string m_deckNobleDeckString;
	std::string m_deckExpansionL1DeckString;
	std::string m_deckExpansionL2DeckString;
	std::string m_deckExpansionL3DeckString;

	// Player
	std::string m_playerPrestigePoints;
};

sf::Packet& operator <<(sf::Packet& packet, const NetworkPacket& networkPacket);

sf::Packet& operator >>(sf::Packet& packet, NetworkPacket& networkPacket);