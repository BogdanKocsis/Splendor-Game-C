#include "NetworkPacket.h"

void NetworkPacket::SetHandData(const std::tuple<std::string, std::string, std::string, std::string>& handData)
{
	auto& [resourcesString, tokensString, expansionString, nobleString] = handData;
	m_handResources = resourcesString;
	m_handTokens = tokensString;
	m_handExpansions = expansionString;
	m_handNoble = nobleString;
}

void NetworkPacket::SetBoardData(const std::tuple<std::string, std::string, std::string, std::string, std::string>& boardData)
{
	auto& [tokensString, nobleSlotsString, expansionL1SlotsString, expansionL2SlotsString, expansionL3SlotsString] = boardData;
	m_boardTokensString = tokensString;
	m_boardNobleSlotsString = nobleSlotsString;
	m_boardExpansionL1SlotsString = expansionL1SlotsString;
	m_boardExpansionL2SlotsString = expansionL2SlotsString;
	m_boardExpansionL3SlotsString = expansionL3SlotsString;
}

void NetworkPacket::SetCardDrawnFromDeck(Board::LastDrawn cardDrawn)
{
	switch (cardDrawn)
	{
		case Board::LastDrawn::None:
			m_cardDrawnFromDeck = "0";
			break;
		case Board::LastDrawn::Level1:
			m_cardDrawnFromDeck = "1";
			break;
		case Board::LastDrawn::Level2:
			m_cardDrawnFromDeck = "2";
			break;
		case Board::LastDrawn::Level3:
			m_cardDrawnFromDeck = "3";
			break;
		default:
			break;
	}
}

void NetworkPacket::SetDecksData(const std::tuple<std::string, std::string, std::string, std::string>& decksData)
{
	auto& [nobleDeckString, expansionL1DeckString, expansionL2DeckString, expansionL3DeckString] = decksData;
	m_deckNobleDeckString = nobleDeckString;
	m_deckExpansionL1DeckString = expansionL1DeckString;
	m_deckExpansionL2DeckString = expansionL2DeckString;
	m_deckExpansionL3DeckString = expansionL3DeckString;
}

void NetworkPacket::SetPlayerData(const std::string& prestigePoints)
{
	m_playerPrestigePoints = prestigePoints;
}

void NetworkPacket::ClearData()
{
	// Hand
	m_handResources = "";
	m_handTokens = "";
	m_handExpansions = "";
	m_handNoble = "";

	// Board
	m_boardTokensString = "";
	m_boardNobleSlotsString = "";
	m_boardExpansionL1SlotsString = "";
	m_boardExpansionL2SlotsString = "";
	m_boardExpansionL3SlotsString = "";
	m_cardDrawnFromDeck = "";

	// Decks
	m_deckNobleDeckString = "";
	m_deckExpansionL1DeckString = "";
	m_deckExpansionL2DeckString = "";
	m_deckExpansionL3DeckString = "";

	// Player
	m_playerPrestigePoints = "";
}

sf::Packet& operator<<(sf::Packet& packet, const NetworkPacket& networkPacket)
{
	//hand data
	packet << networkPacket.m_handResources;
	packet << networkPacket.m_handTokens;
	packet << networkPacket.m_handExpansions;
	packet << networkPacket.m_handNoble;
	//board data
	packet << networkPacket.m_boardTokensString;
	packet << networkPacket.m_boardNobleSlotsString;
	packet << networkPacket.m_boardExpansionL1SlotsString;
	packet << networkPacket.m_boardExpansionL2SlotsString;
	packet << networkPacket.m_boardExpansionL3SlotsString;
	packet << networkPacket.m_cardDrawnFromDeck;
	//decks data
	packet << networkPacket.m_deckNobleDeckString;
	packet << networkPacket.m_deckExpansionL1DeckString;
	packet << networkPacket.m_deckExpansionL2DeckString;
	packet << networkPacket.m_deckExpansionL3DeckString;
	//player data
	packet << networkPacket.m_playerPrestigePoints;
	
	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, NetworkPacket& networkPacket)
{
	//hand data
	packet >> networkPacket.m_handResources;
	packet >> networkPacket.m_handTokens;
	packet >> networkPacket.m_handExpansions;
	packet >> networkPacket.m_handNoble;
	//board data
	packet >> networkPacket.m_boardTokensString;
	packet >> networkPacket.m_boardNobleSlotsString;
	packet >> networkPacket.m_boardExpansionL1SlotsString;
	packet >> networkPacket.m_boardExpansionL2SlotsString;
	packet >> networkPacket.m_boardExpansionL3SlotsString;
	packet >> networkPacket.m_cardDrawnFromDeck;
	//decks data
	packet >> networkPacket.m_deckNobleDeckString;
	packet >> networkPacket.m_deckExpansionL1DeckString;
	packet >> networkPacket.m_deckExpansionL2DeckString;
	packet >> networkPacket.m_deckExpansionL3DeckString;
	//player data
	packet >> networkPacket.m_playerPrestigePoints;
	
	return packet;
}

std::ostream& operator<<(std::ostream& out, const NetworkPacket& packet)
{
	//hand data
	out << "[Hand Data]\n";
	out << "Resources: " << packet.m_handResources << "\n";
	out << "Tokens: " << packet.m_handTokens << "\n";
	out << "Expansions: " << packet.m_handExpansions << "\n";
	out << "Nobles: " << packet.m_handNoble << "\n";
	//board data
	out << "[Board Data]\n";
	out << "Tokens: " << packet.m_boardTokensString << "\n";
	out << "Nobles: " << packet.m_boardNobleSlotsString << "\n";
	out << "ExpansionsL1: " << packet.m_boardExpansionL1SlotsString << "\n";
	out << "ExpansionsL2: " << packet.m_boardExpansionL2SlotsString << "\n";
	out << "ExpansionsL3: " << packet.m_boardExpansionL3SlotsString << "\n";
	out << "CardDrawn: " << packet.m_cardDrawnFromDeck << "\n";
	//decks data
	out << "[Decks Data]\n";
	out << "Nobles: " << packet.m_deckNobleDeckString << "\n";
	out << "ExpansionsL1: " << packet.m_deckExpansionL1DeckString << "\n";
	out << "ExpansionsL2: " << packet.m_deckExpansionL2DeckString << "\n";
	out << "ExpansionsL3: " << packet.m_deckExpansionL3DeckString << "\n";
	//player data
	out << "[Player Data]\n";
	out << "Prestige points:" << packet.m_playerPrestigePoints << "\n";
	
	return out;
}
