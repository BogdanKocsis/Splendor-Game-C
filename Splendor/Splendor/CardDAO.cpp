#include "CardDAO.h"
#include "GamePieces.h"

#include <fstream>
#include <sstream>
#include <thread>

CardDAO::CardDAO()
{
	if (!s_isInitialized)
	{
		s_isInitialized = true;
		InitializeDatabase();
	}
}

CardDAO::Noble CardDAO::GetNoble(uint16_t id)
{
	return s_nobleCards[id];
}

CardDAO::Expansion CardDAO::GetL1Expansion(uint16_t id)
{
	return s_expansionL1Cards[id];
}
CardDAO::Data::Data(Type dataType, uint16_t dataID) :type(dataType), id(dataID) {}

CardDAO::Expansion CardDAO::GetL2Expansion(uint16_t id)
{
	return s_expansionL2Cards[id];
}

CardDAO::Expansion CardDAO::GetL3Expansion(uint16_t id)
{
	return s_expansionL3Cards[id];
}

void CardDAO::InitializeDatabase()
{
	// Parse XML Document (Possible undefined behavior because of the const_cast<char*>)
	std::ifstream databaseFile(s_databasePath);
	std::string* databaseContent = new std::string((std::istreambuf_iterator<char>(databaseFile)),
		std::istreambuf_iterator<char>());
	rapidxml::xml_document<> xmlDocument;
	xmlDocument.parse<0>(const_cast<char*>(databaseContent->c_str()));

	// Read data on separate threads
	rapidxml::xml_node<>* mainRoot = xmlDocument.first_node();
	std::thread* nobleThread = new std::thread(InitializeSpecificData, Type::Noble, mainRoot);
	std::thread* expansionL1Thread = new std::thread(InitializeSpecificData, Type::ExpansionL1, mainRoot);
	std::thread* expansionL2Thread = new std::thread(InitializeSpecificData, Type::ExpansionL2, mainRoot);
	std::thread* expansionL3Thread = new std::thread(InitializeSpecificData, Type::ExpansionL3, mainRoot);
	nobleThread->join();
	expansionL1Thread->join();
	expansionL2Thread->join();
	expansionL3Thread->join();
}

void CardDAO::InitializeSpecificData(Type type, rapidxml::xml_node<>* mainRoot)
{
	// Specifications
	rapidxml::xml_node<>* root;
	uint16_t count;
	switch (type)
	{
	case Type::Noble:
		root = mainRoot->first_node("NOBLE_CARDS");
		count = GamePieces::s_NobleCardCount;
		break;
	case Type::ExpansionL1:
		root = mainRoot->first_node("EXPANSION_CARDS")->first_node("LEVEL1");
		count = GamePieces::s_L1ExpansionCardCount;
		break;
	case Type::ExpansionL2:
		root = mainRoot->first_node("EXPANSION_CARDS")->first_node("LEVEL2");
		count = GamePieces::s_L2ExpansionCardCount;
		break;
	case Type::ExpansionL3:
		root = mainRoot->first_node("EXPANSION_CARDS")->first_node("LEVEL3");
		count = GamePieces::s_L3ExpansionCardCount;
		break;
	default:
		return;
	}

	rapidxml::xml_node<>* currentNode = root->first_node();
	for (uint16_t it = 0; it < count; ++it)
	{
		if (type == Type::Noble)
		{
			//Noble data
			uint16_t id = std::stoi(std::string(currentNode->first_node("ID")->value()));
			std::string name(currentNode->first_node("NAME")->value());
			uint16_t prestige = std::stoi(std::string(currentNode->first_node("PRESTIGE")->value()));
			rapidxml::xml_node<>* requestNode = currentNode->first_node("REQUEST");
			RequestMap request;
			request[GemType::GreenEmerald] = std::stoi(std::string(requestNode->first_node("GE")->value()));
			request[GemType::BlueSapphire] = std::stoi(std::string(requestNode->first_node("BS")->value()));
			request[GemType::WhiteDiamond] = std::stoi(std::string(requestNode->first_node("WD")->value()));
			request[GemType::BlackOnyx] = std::stoi(std::string(requestNode->first_node("BO")->value()));
			request[GemType::RedRuby] = std::stoi(std::string(requestNode->first_node("RR")->value()));

			// Noble initialization
			s_nobleCards[id] = { name, prestige, request };
		}
		else
		{
			//Expansion data
			uint16_t id = std::stoi(std::string(currentNode->first_node("ID")->value()));
			std::string rewardStr(currentNode->first_node("REWARD")->value());
			GemType reward;
			if (rewardStr == "GE")
				reward = GemType::GreenEmerald;
			else if (rewardStr == "BS")
				reward = GemType::BlueSapphire;
			else if (rewardStr == "WD")
				reward = GemType::WhiteDiamond;
			else if (rewardStr == "BO")
				reward = GemType::BlackOnyx;
			else if (rewardStr == "RR")
				reward = GemType::RedRuby;
			else
				return;
			uint16_t prestige = std::stoi(std::string(currentNode->first_node("PRESTIGE")->value()));
			rapidxml::xml_node<>* requestNode = currentNode->first_node("REQUEST");
			RequestMap request;
			request[GemType::GreenEmerald] = std::stoi(std::string(requestNode->first_node("GE")->value()));
			request[GemType::BlueSapphire] = std::stoi(std::string(requestNode->first_node("BS")->value()));
			request[GemType::WhiteDiamond] = std::stoi(std::string(requestNode->first_node("WD")->value()));
			request[GemType::BlackOnyx] = std::stoi(std::string(requestNode->first_node("BO")->value()));
			request[GemType::RedRuby] = std::stoi(std::string(requestNode->first_node("RR")->value()));

			// Expansion initialization
			switch (type)
			{
			case Type::ExpansionL1:
				s_expansionL1Cards[id] = { reward, prestige, request };
				break;
			case Type::ExpansionL2:
				s_expansionL2Cards[id] = { reward, prestige, request };
				break;
			case Type::ExpansionL3:
				s_expansionL3Cards[id] = { reward, prestige, request };
				break;
			default:
				return;
			}
		}
		// Incrementation
		currentNode = currentNode->next_sibling();
	}
}
