#pragma once
#include <SFML/Graphics.hpp>

// GUI
#include "UIInfoPanel.h"
#include "UIPlayersPanel.h"
#include "UIVTokensPanel.h"
#include "UICardsRowPanel.h"
#include "UIHandPanel.h"
#include "UITokenAlertPanel.h"

// Logic
#include "PregameSetup.h"
#include "Board.h"
#include "Player.h"

class Collider;

class UIGameSession :public sf::Drawable
{
public:
	enum class Events
	{
		// possible events hapening in the scene
		None,
		MenuButton,
		PassButton
	};

public:
	UIGameSession(const sf::Vector2u& windowSize, const PregameSetup& pregameSetup, std::vector<Player>* pPlayers, Board* pBoard, std::reference_wrapper<Player>& rActivePlayer);

	// UI Logic
	void StartGame();
	void StopGame();
	void UpdateGame();
	void NextTurn();
	void NextTurnOnline();
	void PointToNextPlayer();
	void SetActivePlayer(std::reference_wrapper<Player> activePlayerReference);
	void SyncBoard();
	void SyncOnlineAdversaryPlayerPanel(uint16_t adversaryPrestigePoints);

	// Events
	void PassEvent(const sf::Event& event);
	Events GetEvent() const;

	// Graphics
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const override;

private:
	// UI Logic
	void ValidateActivePlayerChanges();
	void CheckForExceedingTokens();
	void PrepareUI();

private:
	// GUI
	std::vector<UIPanel*> m_panels;
	UIInfoPanel m_infoPanel;
	UIPlayersPanel m_playersPanel;
	UIVTokensPanel m_tokensPanel;
	UICardsRowPanel m_noblesPanel;
	UICardsRowPanel m_expansionsL1Panel;
	UICardsRowPanel m_expansionsL2Panel;
	UICardsRowPanel m_expansionsL3Panel;
	UIHandPanel m_handPanel;
	UITokenAlertPanel m_tokenAlertPanel;
	std::vector<std::reference_wrapper<UICardsRowPanel>> m_expansionPanels;

	// Logic
	PregameSetup m_pregameSetup;
	//std::shared_ptr<Board> p_board;
	Board* p_board;
	std::reference_wrapper<Player>& r_activePlayer;
	//std::shared_ptr<Hand> p_exceedingHand;
	Hand* p_exceedingHand;

};