#include "pch.h"
#include "Deck.h"

#include "Board.h"
#include "CppUnitTest.h"
#include "GamePieces.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SplendorTests
{
	TEST_CLASS(DeckTests)
	{
	public:
		TEST_METHOD(ExpansionL1DeckInitialization) {
			const Deck<ExpansionCard, 1> m_expansionL1Deck;
			Assert::IsTrue(m_expansionL1Deck.GetNumberOfCards() == GamePieces::s_L1ExpansionCardCount);
		}
		TEST_METHOD(ExpansionL2DeckInitialization) {
			const Deck<ExpansionCard, 2> m_expansionL2Deck;
			Assert::IsTrue(m_expansionL2Deck.GetNumberOfCards() == GamePieces::s_L2ExpansionCardCount);
		}

		TEST_METHOD(ExpansionL3DeckInitialization) {
			const Deck<ExpansionCard, 3> m_expansionL3Deck;
			Assert::IsTrue(m_expansionL3Deck.GetNumberOfCards() == GamePieces::s_L3ExpansionCardCount);
		}

		TEST_METHOD(NobleDeckInitialization) {
			const Deck<NobleCard> m_nobleDeck;
			Assert::IsTrue(m_nobleDeck.GetNumberOfCards() == GamePieces::s_NobleCardCount);
		}

		TEST_METHOD(UndefinitedDeckInitialization) {

			Assert::ExpectException<std::invalid_argument>([]() {const Deck<ExpansionCard, NULL> deck; });
		}

		TEST_METHOD(ShuffleExpansionL1Deck) {
			std::vector<Deck<ExpansionCard, 1>> decks(5);
			for (auto& deck : decks)deck.ShuffleDeck();
			std::vector<ExpansionCard> cards;
			for (auto& deck : decks) cards.emplace_back(deck.DrawCard());

			auto func = [&cards]()->bool
			{	bool var = false;
			for (int i = 0; i < cards.size() - 1; i++)
			{
				for (int j = i + 1; j < cards.size(); j++)
				{
					if (cards.at(i).GetId() != cards.at(j).GetId())
						return  true;
				}
			}
			return false;
			};

			Assert::IsTrue(func());
		}

		TEST_METHOD(ShuffleExpansionL2Deck) {
			std::vector<Deck<ExpansionCard, 2>> decks(5);
			for (auto& deck : decks)deck.ShuffleDeck();
			std::vector<ExpansionCard> cards;
			for (auto& deck : decks) cards.emplace_back(deck.DrawCard());

			auto func = [&cards]()->bool
			{	bool var = false;
			for (int i = 0; i < cards.size() - 1; i++)
			{
				for (int j = i + 1; j < cards.size(); j++)
				{
					if (cards.at(i).GetId() != cards.at(j).GetId())
						return  true;
				}
			}
			return false;
			};

			Assert::IsTrue(func());
		}

		TEST_METHOD(ShuffleExpansionL3Deck) {
			std::vector<Deck<ExpansionCard, 3>> decks(5);
			for (auto& deck : decks)deck.ShuffleDeck();
			std::vector<ExpansionCard> cards;
			for (auto& deck : decks) cards.emplace_back(deck.DrawCard());

			auto func = [&cards]()->bool
			{	bool var = false;
			for (int i = 0; i < cards.size() - 1; i++)
			{
				for (int j = i + 1; j < cards.size(); j++)
				{
					if (cards.at(i).GetId() != cards.at(j).GetId())
						return  true;
				}
			}
			return false;
			};

			Assert::IsTrue(func());
		}
		TEST_METHOD(NobleDeckOverflow) {
			Deck<NobleCard> nobleCard;
			uint16_t amount = GamePieces::s_NobleCardCount + 1;
			const auto func = [&nobleCard, &amount]()
			{
				while (amount)
				{
					nobleCard.DrawCard();
					--amount;
				}
			};
			Assert::ExpectException<std::out_of_range>(func);
		}
	};
}