#include "Hand.h"
#include "GamePieces.h"
#include "NetworkPacket.h"

std::vector<CardDAO::Data> Hand::GetExpansionsData() const
{
	std::vector<CardDAO::Data> dataToReturn;

	for (const auto& slot : m_expansionCards)
	{
		if (slot.has_value())
		{
			switch (slot->GetLevel())
			{
			case ExpansionCard::Level::Level1:
				dataToReturn.emplace_back(CardDAO::Type::ExpansionL1, slot->GetId());
				break;
			case ExpansionCard::Level::Level2:
				dataToReturn.emplace_back(CardDAO::Type::ExpansionL2, slot->GetId());
				break;
			case ExpansionCard::Level::Level3:
				dataToReturn.emplace_back(CardDAO::Type::ExpansionL3, slot->GetId());
				break;
			default:
				break;;
			}
		}
	}

	return dataToReturn;
}

std::vector<CardDAO::Data> Hand::GetNoblesData() const
{
	std::vector<CardDAO::Data> dataToReturn;

	for (const auto& slot : m_nobleCards)
	{
		if (slot.has_value())
		{
			dataToReturn.emplace_back(CardDAO::Type::Noble, slot->GetId());
		}
	}

	return dataToReturn;
}

Hand::GemsMap& Hand::GetResourcesData() const
{
	return m_resources;
}

Hand::GemsMap& Hand::GetTokensData() const
{
	return m_tokens;
}

void Hand::AddResource(GemType type)
{
	++m_resources[type];
}

void Hand::AddToken(GemType type, uint16_t amount)
{
	m_tokens[type] += amount;
}

void Hand::RemoveToken(GemType type, uint16_t amount)
{
	if (m_tokens[type] < amount)
		throw std::out_of_range("Amount of tokens to be removed out of range");
	m_tokens[type] -= amount;
}

void Hand::RemoveTokens(GemsMap&& tokens)
{
	for (uint16_t gemTypeIterator = 0; gemTypeIterator < IToken::s_typeCount; ++gemTypeIterator)
	{
		const auto gemType = static_cast<IToken::Type>(gemTypeIterator);
		if (m_tokens[gemType] < tokens[gemType])
			throw std::out_of_range("Amount of tokens to be removed out of range");
		m_tokens[gemType] -= tokens[gemType];
	}
}

void Hand::AddExpansionCard(ExpansionCard&& card)
{
	for (auto& expansionCard : m_expansionCards)
	{
		if (!expansionCard.has_value())
		{
			expansionCard = std::move(card);
			return;
		}
	}
	throw std::out_of_range("Hand is full, can't add expansion card");
}

void Hand::RemoveExpansionCard(uint16_t id)
{
	for (auto& card : m_expansionCards)
	{
		if (card.has_value())
		{
			if (card.value().GetId() == id)
			{
				card.reset();
				return;
			}
		}
	}
	throw std::invalid_argument("Expansion card with given id not found");
}

void Hand::AddNobleCard(NobleCard&& card)
{
	for (auto& nobleCard : m_nobleCards)
	{
		if (!nobleCard.has_value())
		{
			nobleCard = card;
			return;
		}
	}
	throw std::out_of_range("Hand is full, can't add noble card");
}

Hand::GemsMap Hand::BuyExpansionCard(ExpansionCard&& expansionCard)
{
	// Verify
	GemsMap tokensToReturn;
	uint16_t goldSupplement = 0;
	for (uint16_t gemTypeIterator = 0; gemTypeIterator < IToken::s_typeCount - 1; ++gemTypeIterator)
	{
		const auto gemType = static_cast<IToken::Type>(gemTypeIterator);
		const auto needing = expansionCard.GetCost()[gemType];
		const auto resourcesStock = m_resources[gemType];
		const auto tokensStock = m_tokens[gemType];
		const auto having = resourcesStock + tokensStock;
		if (needing > having)
		{
			goldSupplement += needing - having;
			tokensToReturn[gemType] = tokensStock;
		}
		else if (resourcesStock < needing)
		{
			tokensToReturn[gemType] = needing - resourcesStock;
		}
	}

	// Error
	if (goldSupplement > m_tokens[IToken::Type::Gold])
		throw std::length_error("Not enough resources to buy expansion card");

	// Buy
	m_resources[expansionCard.GetRewardType()]++;
	tokensToReturn[IToken::Type::Gold] = goldSupplement;
	for (uint16_t gemTypeIterator = 0; gemTypeIterator < IToken::s_typeCount; ++gemTypeIterator)
	{
		const auto gemType = static_cast<IToken::Type>(gemTypeIterator);
		m_tokens[gemType] -= tokensToReturn[gemType];
	}
	return tokensToReturn;
}

std::tuple < std::string, std::string, std::string, std::string > Hand::ConvertToPackage() const
{
	// Resources
	std::string resourcesString;
	for (const auto& resource : m_resources)
	{
		switch (resource.first)
		{
		case IToken::Type::GreenEmerald:
			resourcesString += "E";
			break;
		case IToken::Type::BlueSapphire:
			resourcesString += "S";
			break;
		case IToken::Type::WhiteDiamond:
			resourcesString += "D";
			break;
		case IToken::Type::BlackOnyx:
			resourcesString += "O";
			break;
		case IToken::Type::RedRuby:
			resourcesString += "R";
			break;
		case IToken::Type::Gold:
			resourcesString += "G";
			break;
		default:
			throw std::domain_error("Error converting resources to package");
		}
		resourcesString += std::to_string(resource.second);
	}

	// Tokens
	std::string tokensString;
	for (const auto& token : m_tokens)
	{
		switch (token.first)
		{
		case IToken::Type::GreenEmerald:
			tokensString += "E";
			break;
		case IToken::Type::BlueSapphire:
			tokensString += "S";
			break;
		case IToken::Type::WhiteDiamond:
			tokensString += "D";
			break;
		case IToken::Type::BlackOnyx:
			tokensString += "O";
			break;
		case IToken::Type::RedRuby:
			tokensString += "R";
			break;
		case IToken::Type::Gold:
			tokensString += "G";
			break;
		default:
			throw std::domain_error("Error converting tokens to package");
		}
		tokensString += std::to_string(token.second);
	}

	// Expansion Cards
	std::string expansionCardsString;
	for (const auto& expansionCard : m_expansionCards)
	{
		if (expansionCard.has_value())
		{
			switch (expansionCard.value().GetLevel())
			{
			case ExpansionCard::Level::Level1:
				expansionCardsString += "1";
				break;
			case ExpansionCard::Level::Level2:
				expansionCardsString += "2";
				break;
			case ExpansionCard::Level::Level3:
				expansionCardsString += "3";
				break;
			default:
				throw std::domain_error("Error converting expansion cards to package");;
			}
			expansionCardsString += std::to_string(expansionCard.value().GetId()) + '-';
		}
	}

	// Noble Cards
	std::string nobleCardsString;
	for (const auto& nobleCard : m_nobleCards)
	{
		if (nobleCard.has_value())
		{
			nobleCardsString += std::to_string(nobleCard.value().GetId()) + '-';
		}
	}

	// Return package
	return std::make_tuple(resourcesString, tokensString, expansionCardsString, nobleCardsString);
}

void Hand::ConvertFromPackage(NetworkPacket& networkPacket)
{
	// Resources
	NetworkPacket::TokenizePackage(m_resources, std::move(networkPacket.m_handResources));

	// Tokens
	NetworkPacket::TokenizePackage(m_tokens, std::move(networkPacket.m_handTokens));

	// Expansions
	NetworkPacket::TokenizePackage(m_expansionCards, std::move(networkPacket.m_handExpansions));

	// Nobles
	NetworkPacket::TokenizePackage(m_nobleCards, std::move(networkPacket.m_handNoble));
}

bool Hand::IsFull() const
{
	return m_expansionCards[2].has_value();
}

bool Hand::ExceedsTokenLimit() const
{
	uint16_t tokenCount = 0;

	for (const auto& token : m_tokens)
	{
		tokenCount += token.second;
	}

	if (tokenCount > GamePieces::s_HeldTokensLimit)
		return true;
	return false;
}
