#pragma once
#include "UIPanel.h"
#include "UICard.h"
#include "CardDAO.h"

#include <optional>

class UICardsRowPanel :public UIPanel
{
public:
	UICardsRowPanel(uint16_t cardSlots, const sf::Vector2f& position = { 0,0 }, const sf::Vector2f& size = { 1024,100 }, const sf::Vector2f& padding = { 0.1f,0.05f }, bool isActive = true);
	
	// UI Logic
	std::vector<UICard::Data> GetCardsData() const;
	void SetCardsData(const std::vector<UICard::Data>& cardsData);
	void SetCardsData(const std::vector<CardDAO::Data>& cardsData, uint16_t withBackground = 0, bool isDataNumb = false);
	std::optional<std::pair<UICard*, UICard::State>> CheckForPickedCard();
	std::optional<UICard::Data> CheckForWonNoble(std::unordered_map<IToken::Type, uint16_t>& resources);
	void NumbAll();
	void UnNumbAll();
	void DisableDeckBackground();
	void InHandAll();

	// GUI
	void ReverseDrawOrder();

private:
	uint16_t m_cardSlots;
	sf::Vector2f m_cardSize;
	float m_cardDistance;
	std::vector<UICard*> m_cards;

};

