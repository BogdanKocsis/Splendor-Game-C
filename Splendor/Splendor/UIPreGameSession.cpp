#include "UIPreGameSession.h"

UIPreGameSession::UIPreGameSession(const sf::Vector2u& windowSize) :
	// Game Mode Panel
	m_gameModePanel("Game Mode: ", UIOptionsPanel::Type::Radio,
		sf::Vector2f(50.0f, 0.2f * windowSize.y + 30.0f),
		sf::Vector2f(windowSize.x - 50.0f, 0.15f * windowSize.y)),
	// Players Panel
	m_playersPanel("Players: ", UIOptionsPanel::Type::Radio,
		sf::Vector2f(50.0f, 0.4f * windowSize.y + 50.0f),
		sf::Vector2f(windowSize.x - 50.0f, 0.15f * windowSize.y)),
	// Other Settings Panel
	m_otherSettingsPanel("Other: ", UIOptionsPanel::Type::Check,
		sf::Vector2f(50.0f, 0.6f * windowSize.y + 70.0f),
		sf::Vector2f(windowSize.x - 50.0f, 0.15f * windowSize.y))
{
	// Title Background
	m_titleBackground.setPosition(0, 0);
	m_titleBackground.setSize(sf::Vector2f(windowSize.x, 0.2f * windowSize.y));
	m_titleBackground.setFillColor(UIColors::DarkYellow);

	// Title
	m_title.setString("New Game Setup");
	m_title.setCharacterSize(80);
	m_title.setFillColor(UIColors::NeutralWhite);
	m_title.setPosition(windowSize.x / 2.0f, 0.2f * windowSize.y / 2.0f);
	m_title.AlignText(UIText::TextAlign::mid_center);

	// GameMode Panel
	m_gameModePanel.AddOption("Offline");
	m_gameModePanel.AddOption("Client");
	m_gameModePanel.AddOption("Server");

	// Players Panel
	m_playersPanel.AddOption("2");
	m_playersPanel.AddOption("3");
	m_playersPanel.AddOption("4");

	// Other Settings Panel
	m_otherSettingsPanel.AddOption("Timer");
	m_otherSettingsPanel.AddOption("A.I.");

	//Button
	sf::RectangleShape button({ 300,80 });

	// Start Game Button
	button.setPosition(windowSize.x - 350.0f, windowSize.y - 130.0f);
	m_startGameButton = new UIButton(button);
	m_startGameButton->ChangeText("Start Game");

	// Main Menu Button
	button.setPosition(windowSize.x - 670.0f, windowSize.y - 130.0f);
	m_mainMenuButton = new UIButton(button);
	m_mainMenuButton->ChangeText("Main Menu");
}

UIPreGameSession::~UIPreGameSession()
{
	delete m_startGameButton;
	delete m_mainMenuButton;
}

void UIPreGameSession::PassEvent(const sf::Event& event)
{
	m_gameModePanel.HandleEvent(event);
	m_playersPanel.HandleEvent(event);
	m_otherSettingsPanel.HandleEvent(event);
	m_startGameButton->HandleEvent(event);
	m_mainMenuButton->HandleEvent(event);
}

UIPreGameSession::Events UIPreGameSession::GetEvent() const
{
	if (m_mainMenuButton->GetState() == UIButton::State::Release)
	{
		return Events::MainMenu;
	}
	if (m_startGameButton->GetState() == UIButton::State::Release)
	{
		return Events::StartGame;
	}
	return Events::None;
}

void UIPreGameSession::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_titleBackground);
	target.draw(m_title);
	target.draw(m_gameModePanel);
	target.draw(m_playersPanel);
	target.draw(m_otherSettingsPanel);
	target.draw(*m_startGameButton);
	target.draw(*m_mainMenuButton);
}

PregameSetup UIPreGameSession::GetPregameSetup() const
{
	// Player Count
	const uint16_t playerCount = std::stoi(m_playersPanel.FirstChecked());

	// Game Mode
	std::string gameModeOptionString = m_gameModePanel.FirstChecked();
	PregameSetup::GameMode gameMode;
	if (gameModeOptionString == "Offline")
	{
		gameMode = PregameSetup::GameMode::Offline;
	}
	else if (gameModeOptionString == "Client")
	{
		gameMode = PregameSetup::GameMode::Client;
	}
	else if (gameModeOptionString == "Server")
	{
		gameMode = PregameSetup::GameMode::Server;
	}
	else
	{
		throw std::invalid_argument("Invalid game mode");
	}

	// Other
	const bool withTimer = m_otherSettingsPanel.IsChecked("Timer");
	const bool withAI = m_otherSettingsPanel.IsChecked("A.I.");

	return PregameSetup(playerCount, gameMode, withTimer, withAI);
}
