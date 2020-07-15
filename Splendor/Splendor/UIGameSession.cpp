#include "UIGameSession.h"
#include "UISelectedCard.h"
#include "SoundSystem.h"

#include <iostream>

UIGameSession::UIGameSession(const sf::Vector2u& windowSize, const PregameSetup& pregameSetup, std::vector<Player>* pPlayers, Board* pBoard, std::reference_wrapper<Player>& rActivePlayer) :
	// instantiate Logic
	p_board(pBoard),
	r_activePlayer(rActivePlayer),
	// instantiate UI panels
	m_infoPanel(sf::Vector2f(0, 0), sf::Vector2f(windowSize.x, windowSize.y * 0.05)),
	m_playersPanel(pPlayers, sf::Vector2f(0, windowSize.y * 0.05), sf::Vector2f(windowSize.x * 0.3, windowSize.y * 0.95)),
	m_tokensPanel(sf::Vector2f(windowSize.x * 0.3, windowSize.y * 0.05), sf::Vector2f(0.1 * windowSize.x, windowSize.y * 0.95)),
	m_noblesPanel(5, sf::Vector2f(windowSize.x * 0.4, windowSize.y * 0.05), sf::Vector2f(windowSize.x * 0.6, windowSize.y * 0.26)),
	m_expansionsL3Panel(5, sf::Vector2f(windowSize.x * 0.4, windowSize.y * 0.31), sf::Vector2f(windowSize.x * 0.6, windowSize.y * 0.23)),
	m_expansionsL2Panel(5, sf::Vector2f(windowSize.x * 0.4, windowSize.y * 0.54), sf::Vector2f(windowSize.x * 0.6, windowSize.y * 0.23)),
	m_expansionsL1Panel(5, sf::Vector2f(windowSize.x * 0.4, windowSize.y * 0.77), sf::Vector2f(windowSize.x * 0.6, windowSize.y * 0.23)),
	m_handPanel(static_cast<sf::Vector2f>(windowSize), false),
	m_tokenAlertPanel(static_cast<sf::Vector2f>(windowSize), false),
	p_exceedingHand(nullptr),
	// instantiate Pregame Set-up
	m_pregameSetup(pregameSetup)
{
	// Instantiate UI Card Rows
	m_noblesPanel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::Noble), 0, true);
	m_noblesPanel.ReverseDrawOrder();
	m_expansionsL3Panel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::ExpansionL3), 3);
	m_expansionsL3Panel.ReverseDrawOrder();
	m_expansionsL2Panel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::ExpansionL2), 2);
	m_expansionsL2Panel.ReverseDrawOrder();
	m_expansionsL1Panel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::ExpansionL1), 1);
	m_expansionsL1Panel.ReverseDrawOrder();
	m_expansionPanels.emplace_back(std::ref(m_expansionsL3Panel));
	m_expansionPanels.emplace_back(std::ref(m_expansionsL2Panel));
	m_expansionPanels.emplace_back(std::ref(m_expansionsL1Panel));

	// Populate panel vector
	m_panels.push_back(dynamic_cast<UIPanel*>(&m_infoPanel));
	m_panels.push_back(dynamic_cast<UIPanel*>(&m_playersPanel));
	m_panels.push_back(dynamic_cast<UIPanel*>(&m_tokensPanel));
	m_panels.push_back(dynamic_cast<UIPanel*>(&m_expansionsL1Panel));
	m_panels.push_back(dynamic_cast<UIPanel*>(&m_expansionsL2Panel));
	m_panels.push_back(dynamic_cast<UIPanel*>(&m_expansionsL3Panel));
	m_panels.push_back(dynamic_cast<UIPanel*>(&m_noblesPanel));
	m_panels.push_back(dynamic_cast<UIPanel*>(&m_tokenAlertPanel));
	m_panels.push_back(dynamic_cast<UIPanel*>(&m_handPanel));

}

void UIGameSession::StartGame()
{
	m_infoPanel.StartTimer();
}

void UIGameSession::StopGame()
{
	m_infoPanel.StopTimer();
}

void UIGameSession::UpdateGame()
{
	// Info
	m_infoPanel.UpdateTime();

	// Tokens Panel
	m_tokensPanel.UpdateTokens(p_board->GetTokensData());
	if (m_tokensPanel.GetLastPicked().has_value())
	{
		auto& token = m_tokensPanel.GetLastPicked();
		r_activePlayer.get().GetHand().AddToken(token.value());
		p_board->TakeToken(token.value());
		token.reset();
	}
	if (m_tokensPanel.GetHasPicked())
	{
		m_tokensPanel.SetHasPicked(false);
		std::for_each(m_expansionPanels.begin(), m_expansionPanels.end(), [](std::reference_wrapper<UICardsRowPanel>& panel) {panel.get().NumbAll(); });
	}

	// Card Panels
	std::optional<std::pair<UICard*, UICard::State>> pickedCard;
	for (auto& expansionPanel : m_expansionPanels)
	{
		if (pickedCard.has_value())
			continue;
		pickedCard = expansionPanel.get().CheckForPickedCard();
	}
	if (pickedCard.has_value())
	{
		switch (pickedCard.value().second)
		{
		case UICard::State::LeftRelease:/// Buy Card
			std::cout << "Picked card LEFT CLICK\n";
			// Background
			if (pickedCard->first->GetType() == UICard::Type::Background)
				break;
			// Expansion Card
			try
			{
				// Create logic ExpansionCard for pickedCard
				ExpansionCard pickedCardLogicPiece(static_cast<ExpansionCard::Level>(static_cast<uint16_t>(pickedCard.value().first->GetType())), pickedCard.value().first->GetID());

				// Save picked card data
				const auto prestigePoints = pickedCardLogicPiece.GetPrestigePoints();
				const auto level = pickedCardLogicPiece.GetLevel();
				const auto id = pickedCardLogicPiece.GetId();

				// Return & sync tokens
				p_board->ReturnTokens(r_activePlayer.get().GetHand().BuyExpansionCard(std::move(pickedCardLogicPiece)));
				m_tokensPanel.SyncTokens(p_board->GetTokensData());

				// Replace card in board
				p_board->ReplaceExpansion(level, id);

				// Add card's prestige points
				r_activePlayer.get().AddPrestigePoints(prestigePoints);
				m_playersPanel.AddPrestigePointsToCurrentPlayer(prestigePoints);

				// Deactivate UI
				pickedCard->first->Deactivate();
				m_tokensPanel.NumbAll();
				std::for_each(m_expansionPanels.begin(), m_expansionPanels.end(), [](std::reference_wrapper<UICardsRowPanel>& panel) {panel.get().NumbAll(); });

				// Sound SFX
				SoundSystem::PlaySFX(SoundSystem::SoundType::BuyCardSFX);
			}
			catch (std::length_error & exception)
			{
				// Not enough resources
				std::cout << exception.what() << "\n";
				pickedCard->first->TriggerWarning();
			}
			catch (std::exception & exception)
			{
				// Other possible exception
				std::cout << exception.what() << "\n";
			}
			break;
		case UICard::State::RightRelease:/// Hold Card
			std::cout << "Picked card RIGHT CLICK\n";
			// Background
			if (pickedCard->first->GetType() == UICard::Type::Background)
			{
				// Hand is full
				if (r_activePlayer.get().GetHand().IsFull())
				{
					pickedCard->first->TriggerWarning();
					break;
				}

				// Transfer expansion card from board to active player hand
				try {
					r_activePlayer.get().GetHand().AddExpansionCard(p_board->DrawExpansionFromDeck(pickedCard->first->GetID()));
				}
				catch (std::out_of_range & exception)
				{
					// Empty Deck
					std::cout << exception.what() << "\n";
					pickedCard.value().first->TriggerWarning();
					break;
				}

				// Pick a gold token
				try {
					p_board->TakeToken(IToken::Type::Gold);
					r_activePlayer.get().GetHand().AddToken(IToken::Type::Gold);
					m_tokensPanel.TakeGoldToken();
				}
				catch (std::out_of_range & exception)
				{
					// Not enough gold tokens on board
				}

				// Deactivate UI
				pickedCard->first->OnMouseLeave();
				m_tokensPanel.NumbAll();
				std::for_each(m_expansionPanels.begin(), m_expansionPanels.end(), [](std::reference_wrapper<UICardsRowPanel>& panel) {panel.get().NumbAll(); });

				// Sound SFX
				SoundSystem::PlaySFX(SoundSystem::SoundType::HoldCardSFX);

				break;
			}
			// Expansion Card
			try {
				// Create logic ExpansionCard for pickedCard
				ExpansionCard pickedCardLogicPiece(static_cast<ExpansionCard::Level>(static_cast<uint16_t>(pickedCard.value().first->GetType())), pickedCard.value().first->GetID());

				// Save picked card data
				const auto level = pickedCardLogicPiece.GetLevel();
				const auto id = pickedCardLogicPiece.GetId();

				// Transfer card to hand
				r_activePlayer.get().GetHand().AddExpansionCard(std::move(pickedCardLogicPiece));

				// Replace card in board
				p_board->ReplaceExpansion(level, id);

				// Pick a gold token
				try {
					p_board->TakeToken(IToken::Type::Gold);
					r_activePlayer.get().GetHand().AddToken(IToken::Type::Gold);
					m_tokensPanel.TakeGoldToken();
				}
				catch (std::out_of_range & exception)
				{
					// Not enough gold tokens on board
				}

				// Deactivate UI
				pickedCard->first->Deactivate();
				m_tokensPanel.NumbAll();
				std::for_each(m_expansionPanels.begin(), m_expansionPanels.end(), [](std::reference_wrapper<UICardsRowPanel>& panel) {panel.get().NumbAll(); });

				// Sound SFX
				SoundSystem::PlaySFX(SoundSystem::SoundType::HoldCardSFX);
			}
			catch (std::out_of_range & exception)
			{
				// Hand Full
				std::cout << exception.what() << "\n";
				pickedCard->first->TriggerWarning();
			}
			break;
		default:
			break;
		}
		std::cout << "\n";
	}

	// Player Hand Panel
	const auto triggeredPanel = m_playersPanel.GetIfTriggered();
	if (triggeredPanel != nullptr)
	{
		std::for_each(m_panels.begin(), m_panels.end(), [](UIPanel* panel) {panel->SetInteractable(false); });
		m_handPanel.SetUpHand(*triggeredPanel);
		m_handPanel.SetActive(true);
		if (triggeredPanel->GetPlayer()->GetId() == r_activePlayer.get().GetId()) m_handPanel.SetInteractable(true);
	}
	if (m_handPanel.CheckForClose())// closed
	{
		std::for_each(m_panels.begin(), m_panels.end() - 1, [](UIPanel* panel) {panel->SetInteractable(true); });
	}
	else if (m_handPanel.IsActive())// opened
	{
		if (!m_tokensPanel.IsNumb())// allowed to buy
		{
			auto pickedExpansion = m_handPanel.GetPickedExpansion();
			if (pickedExpansion.has_value())
			{
				// Buy ExpansionCard from hand
				try {
					// Create logic ExpansionCard for pickedCard
					ExpansionCard pickedCardLogicPiece(static_cast<ExpansionCard::Level>(static_cast<uint16_t>(pickedExpansion.value()->GetType())), pickedExpansion.value()->GetID());

					// Save picked card data
					const auto prestigePoints = pickedCardLogicPiece.GetPrestigePoints();

					// Return & sync tokens
					p_board->ReturnTokens(r_activePlayer.get().GetHand().BuyExpansionCard(std::move(pickedCardLogicPiece)));
					m_tokensPanel.SyncTokens(p_board->GetTokensData());

					// Add card's prestige points
					r_activePlayer.get().AddPrestigePoints(prestigePoints);
					m_playersPanel.AddPrestigePointsToCurrentPlayer(prestigePoints);

					// Sync Hand
					r_activePlayer.get().GetHand().RemoveExpansionCard(pickedExpansion.value()->GetID());
					m_handPanel.SyncHand();

					// Deactivate UI
					pickedExpansion.value()->Deactivate();
					m_handPanel.NumbAllExpansions();
					m_tokensPanel.NumbAll();
					std::for_each(m_expansionPanels.begin(), m_expansionPanels.end(), [](std::reference_wrapper<UICardsRowPanel>& panel) {panel.get().NumbAll(); });

					// Sound SFX
					SoundSystem::PlaySFX(SoundSystem::SoundType::BuyCardSFX);
				}
				catch (std::length_error & exception)
				{
					// Not enough resources
					std::cout << exception.what() << "\n";
					pickedExpansion.value()->TriggerWarning();
				}
			}
		}
		else// not allowed to buy
		{
			m_handPanel.NumbAllExpansions();
		}
	}

	// Token Alert Panel
	if (m_tokensPanel.IsActive())
	{
		m_tokenAlertPanel.Update();

		if (m_tokenAlertPanel.GetConfirmed())
		{
			// Transfer tokens
			p_board->ReturnTokens(m_tokenAlertPanel.GetTokensToReturn());
			p_exceedingHand->RemoveTokens(m_tokenAlertPanel.GetTokensToReturn());
			p_exceedingHand = nullptr;
			m_tokensPanel.SyncTokens(p_board->GetTokensData());

			// UI
			std::for_each(m_panels.begin(), m_panels.end() - 1, [](UIPanel* panel) {panel->SetInteractable(true); });
			m_tokenAlertPanel.SetConfirmed(false);
			m_tokenAlertPanel.SetActive(false);
			m_tokenAlertPanel.SetInteractable(false);
		}
	}
}

void UIGameSession::NextTurn()
{
	// Validate Active Player Changes
	ValidateActivePlayerChanges();
	CheckForExceedingTokens();

	// UI Logic
	m_infoPanel.IncrementTurn();
	PointToNextPlayer();
	PrepareUI();
}

void UIGameSession::NextTurnOnline()
{
	// Validate Active Player Changes
	ValidateActivePlayerChanges();
	CheckForExceedingTokens();

	// UI Logic
	m_infoPanel.IncrementTurn();
	PrepareUI();
}

void UIGameSession::PointToNextPlayer()
{
	m_playersPanel.PointToNextPlayer();
}

void UIGameSession::CheckForExceedingTokens()
{
	// Check if active player token stock exceeds token limit
	if (r_activePlayer.get().GetHand().ExceedsTokenLimit())
	{
		std::for_each(m_panels.begin(), m_panels.end() - 1, [](UIPanel* panel) {panel->SetInteractable(false); });
		m_tokenAlertPanel.SetActive(true);
		m_tokenAlertPanel.SetInteractable(true);

		p_exceedingHand = &r_activePlayer.get().GetHand();
		m_tokenAlertPanel.SetInitialTokens(p_exceedingHand->GetTokensData());
	}
}

void UIGameSession::SetActivePlayer(std::reference_wrapper<Player> activePlayerReference)
{
	r_activePlayer = activePlayerReference;
}

void UIGameSession::SyncBoard()
{
	m_tokensPanel.SyncTokens(p_board->GetTokensData());
	m_noblesPanel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::Noble));
	m_expansionsL3Panel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::ExpansionL3), 3);
	m_expansionsL2Panel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::ExpansionL2), 2);
	m_expansionsL1Panel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::ExpansionL1), 1);
}

void UIGameSession::SyncOnlineAdversaryPlayerPanel(uint16_t adversaryPrestigePoints)
{
	m_playersPanel.SyncAdversaryPlayerPrestigePoints(adversaryPrestigePoints);
}

void UIGameSession::PassEvent(const sf::Event& event)
{
	// iterate panel vector and handle events
	std::for_each(m_panels.begin(), m_panels.end(), [&event](UIPanel* panel)
	{
		panel->HandleEvent(event);
	});
}

UIGameSession::Events UIGameSession::GetEvent() const
{
	// Info Panel
	if (m_infoPanel.MenuButtonTriggered())
		return Events::MenuButton;
	if (m_infoPanel.PassButtonTriggered())
		return Events::PassButton;
	return Events::None;
}

void UIGameSession::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// iterate panel vector and draw panels
	std::for_each(m_panels.begin(), m_panels.end(), [&target](UIPanel* panel)
		{
			target.draw(*panel);
		});
	// re-draw selected card on top of all drawables
	if (UISelectedCard::Get().first != nullptr)
	{
		target.draw(*UISelectedCard::Get().first);
	}
	// draw selected card text
	if (UISelectedCard::Get().second != nullptr)
	{
		target.draw(*UISelectedCard::Get().second);
	}
}

void UIGameSession::ValidateActivePlayerChanges()
{
	// Nobles
	const auto wonNoble = m_noblesPanel.CheckForWonNoble(r_activePlayer.get().GetHand().GetResourcesData());
	if (wonNoble.has_value())
	{
		auto&& nobleCard = p_board->WinNoble(wonNoble.value().id);
		m_noblesPanel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::Noble), 0, true);
		r_activePlayer.get().AddPrestigePoints(nobleCard.GetPrestigePoints());
		m_playersPanel.AddPrestigePointsToCurrentPlayer(nobleCard.GetPrestigePoints());
		r_activePlayer.get().GetHand().AddNobleCard(std::move(nobleCard));
		SoundSystem::PlaySFX(SoundSystem::SoundType::WinNobleSFX);
		std::cout << "WON NOBLE\n";
	}

	// Reset picked Tokens buffer
	auto& pickedTokens = m_tokensPanel.ExtractPickedTokens();
	for (auto& token : pickedTokens)
	{
		if (token.has_value())
		{
			token.reset();
		}
	}
}

void UIGameSession::PrepareUI()
{
	// Prepare UI for next turn
	m_tokensPanel.UnNumb();
	m_expansionsL3Panel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::ExpansionL3), 3);
	m_expansionsL2Panel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::ExpansionL2), 2);
	m_expansionsL1Panel.SetCardsData(p_board->GetCardSlotsData(CardDAO::Type::ExpansionL1), 1);
	if (p_board->IsExpansionDeckEmpty(3))
	{
		m_expansionsL3Panel.DisableDeckBackground();
	}
	if (p_board->IsExpansionDeckEmpty(2))
	{
		m_expansionsL2Panel.DisableDeckBackground();
	}
	if (p_board->IsExpansionDeckEmpty(1))
	{
		m_expansionsL1Panel.DisableDeckBackground();
	}
}
