#pragma once
#include "IToken.h"
#include "rapidxml.hpp"

#include <string>
#include <unordered_map>

class CardDAO
{
public:
	using GemType = IToken::Type;
	using RequestMap = std::unordered_map<GemType, uint16_t>;

	enum class Type
	{
		Unknown = -1,
		Noble = 0,
		ExpansionL1 = 1,
		ExpansionL2 = 2,
		ExpansionL3 = 3
	};

	struct Noble
	{
		std::string name;
		uint16_t prestige = 0;
		RequestMap request;
	};

	struct Data
	{
		Data(Type dataType = Type::Unknown, uint16_t dataID = 0);

		Type type;
		uint16_t id;
	};

	struct Expansion
	{
		GemType reward = GemType::GreenEmerald;
		uint16_t prestige = 0;
		RequestMap request;
	};

	using NobleMap = std::unordered_map<uint16_t, Noble>;
	using ExpansionMap = std::unordered_map<uint16_t, Expansion>;

public:
	// Constr. = initializes database
	CardDAO();

	// Manipulators
	static Noble GetNoble(uint16_t id);
	static Expansion GetL1Expansion(uint16_t id);
	static Expansion GetL2Expansion(uint16_t id);
	static Expansion GetL3Expansion(uint16_t id);

private:
	// Database initializers
	static void InitializeDatabase();
	static void InitializeSpecificData(Type type, rapidxml::xml_node<>* mainRoot);

private:
	static inline bool s_isInitialized = false;
	static const inline std::string s_databasePath = "../external/Cards Database/CardsDatabase.xml";
	static inline NobleMap s_nobleCards;
	static inline ExpansionMap s_expansionL1Cards;
	static inline ExpansionMap s_expansionL2Cards;
	static inline ExpansionMap s_expansionL3Cards;
};
