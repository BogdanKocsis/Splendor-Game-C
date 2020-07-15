#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SessionsManager.h"
#include "PregameSetup.h"
#include "UIColors.h"
#include "UIMainMenuSession.h"
#include "UIPreGameSession.h"
#include "UIGameSession.h"
#include "UITutorialSession.h"
#include "UILeaderboardSession.h"
#include "UISettingsSession.h"
#include "CardDAO.h"
#include "Player.h"
#include "SoundSystem.h"
#include "Network.h"
#include "NetworkPacket.h"


SessionsManager::SessionsManager() :
	logger(logFileStream, Logger::Level::Info),
	logFileStream(s_logFile, std::ios::app)
{
	logger.Log("Application started", Logger::Level::Info);
	const sf::VideoMode desktopVM = sf::VideoMode().getDesktopMode();
	const sf::VideoMode windowedVM = sf::VideoMode(1280, 720);
	//const sf::VideoMode windowedVM = sf::VideoMode(1366,768);
	//const sf::VideoMode windowedVM = sf::VideoMode(1600,900);

	window = new sf::RenderWindow(desktopVM, "Splendor", sf::Style::None);
	//window = new sf::RenderWindow(windowedVM, "Splendor", sf::Style::None);

	// Hides the cursor
	window->setMouseCursorVisible(false);

	logger.Log("Window created", Logger::Level::Info);

	windowPosition = window->getPosition();
	windowSize = window->getSize();
}

SessionsManager::~SessionsManager()
{
	delete window;
	logger.Log("Window deleted\n\n", Logger::Level::Info);
}

void SessionsManager::MainMenuSession() const
{
	logger.Log("Entered Main Menu Session", Logger::Level::Info);
	UIMainMenuSession mainMenuSessionGUI(windowSize);
	UISettingsSession settingsSessionGUI(windowSize);
	logger.Log("Initialized Main Menu GUI", Logger::Level::Info);

	// Set-up sound system and play main menu music

	SoundSystem::LoadFromFile();
	SoundSystem::PlayMusic(SoundSystem::MusicType::MenuMusic);



	// Load image and create sprite
	cursorTexture.loadFromFile(s_cursorTexture);
	cursorSprite.setTexture(cursorTexture);

	while (window->isOpen())
	{
		settingsSessionGUI.UpdateSound();
		sf::Event event;
		while (window->pollEvent(event))
		{
			mainMenuSessionGUI.PassEvent(event);
			switch (mainMenuSessionGUI.GetEvent())
			{
			case UIMainMenuSession::Events::NewGame:
				logger.Log("Starting PreGame Session...", Logger::Level::Info);
				PreGameSession();
				break;
			case UIMainMenuSession::Events::Tutorial:
				// Tutorial Session
				logger.Log("Starting Tutorial Session...", Logger::Level::Info);
				TutorialSession();
				break;
			case UIMainMenuSession::Events::Settings:
				// Settings Session
				logger.Log("Starting Settings Session...", Logger::Level::Info);
				SettingsSession();
				break;

			case UIMainMenuSession::Events::Exit:
				SoundSystem::StopMusic(SoundSystem::MusicType::MenuMusic);
				logger.Log("Exiting Main Menu Session...", Logger::Level::Info);
				return;
			case UIMainMenuSession::Events::Test:
				TestSession();
				break;
			case UIMainMenuSession::Events::Leaderboard:
				Leaderboard();
				break;
			default:
				break;
			}
		}

		// Set sprite position to cursor position
		cursorSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)));

		window->clear();
		window->draw(mainMenuSessionGUI);
		window->draw(cursorSprite);
		window->display();
	}
}

void SessionsManager::PreGameSession() const
{
	logger.Log("Entered PreGame Session", Logger::Level::Info);
	UIPreGameSession pregameSessionGUI(windowSize);
	logger.Log("Initialized PreGame GUI", Logger::Level::Info);

	// Load image and create sprite
	cursorTexture.loadFromFile(s_cursorTexture);
	cursorSprite.setTexture(cursorTexture);

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			pregameSessionGUI.PassEvent(event);
			switch (pregameSessionGUI.GetEvent())
			{
			case UIPreGameSession::Events::MainMenu:
			{
				logger.Log("Exiting PreGame Session...", Logger::Level::Info);
				return;
			}
			case UIPreGameSession::Events::StartGame:
			{
				logger.Log("Starting Game Session...", Logger::Level::Info);
				const PregameSetup& pregameSetup = pregameSessionGUI.GetPregameSetup();
				if (pregameSetup.GetGameMode() == PregameSetup::GameMode::Offline)
					GameSessionOffline(pregameSetup);
				else
					GameSessionOnline(pregameSetup);
				return;
			}
			default:
				break;
			}
		}

		// Set sprite position to cursor position
		cursorSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)));

		window->clear(UIColors::NavyBlue);
		window->draw(pregameSessionGUI);
		window->draw(cursorSprite);
		window->display();
	}
}

void SessionsManager::TutorialSession() const
{
	logger.Log("Entered Tutorial Session", Logger::Level::Info);
	UITutorialSession tutorialSessionGUI(windowSize);
	logger.Log("Initialized Tutorial GUI", Logger::Level::Info);

	// Load image and create sprite
	cursorTexture.loadFromFile(s_cursorTexture);
	cursorSprite.setTexture(cursorTexture);

	UITutorialSession::LoadFromFile();
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			tutorialSessionGUI.PassEvent(event);
			switch (tutorialSessionGUI.GetEvent())
			{
			case UITutorialSession::Events::MainMenu:
				logger.Log("Exiting Tutorial Session...", Logger::Level::Info);
				return;
			case UITutorialSession::Events::Next:
				UITutorialSession::IncrementSprite();
				break;
			case UITutorialSession::Events::Previous:
				UITutorialSession::DecrementSprite();
				break;
			default:
				break;
			}
		}

		// Set sprite position to cursor position
		cursorSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)));

		window->clear(UIColors::NavyBlue);
		window->draw(tutorialSessionGUI);
		window->draw(UITutorialSession::GetSprite());
		window->draw(cursorSprite);
		window->display();
	}
}

void SessionsManager::SettingsSession() const
{
	logger.Log("Entered Settings Session", Logger::Level::Info);
	UISettingsSession settingsSessionGUI(windowSize);
	logger.Log("Initialized Settings GUI", Logger::Level::Info);

	// Load image and create sprite
	cursorTexture.loadFromFile(s_cursorTexture);
	cursorSprite.setTexture(cursorTexture);

	while (window->isOpen())
	{

		settingsSessionGUI.UpdateSound();
		sf::Event event;
		while (window->pollEvent(event))
		{


			settingsSessionGUI.PassEvent(event);
			switch (settingsSessionGUI.GetEvent())
			{
			case UISettingsSession::Events::MainMenu:

				logger.Log("Exiting PreGame Session...", Logger::Level::Info);
				return;

			default:
				break;
			}
		}

		// Set sprite position to cursor position
		cursorSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)));

		window->clear(UIColors::NavyBlue);
		window->draw(settingsSessionGUI);
		window->draw(cursorSprite);
		window->display();
	}


}

void SessionsManager::GameSessionOffline(const PregameSetup& pregameSetup) const
{
	logger.Log("Entered Game Session", Logger::Level::Info);

	// Initialize Database
	CardDAO cardsDatabase;
	logger.Log("Initialized Cards Database", Logger::Level::Info);

	//Game Music sound on
	SoundSystem::StopMusic(SoundSystem::MusicType::MenuMusic);
	SoundSystem::PlayMusic(SoundSystem::MusicType::GameMusic);
	SoundSystem::SetMusicVolume(60);

	// Initialize Players
	std::vector<Player> players;
	for (size_t playerNr = 1; playerNr <= pregameSetup.GetPlayerCount(); ++playerNr)
	{
		players.emplace_back(playerNr, "Player " + std::to_string(playerNr));
	}
	if (players.size() >= 2)
	{
		players[0].SetName("Adrian");
		players[1].SetName("Bogdan");
	}
	if (players.size() >= 3)
	{
		players[2].SetName("Eugen");
	}
	if (players.size() == 4)
	{
		players[3].SetName("Teodor");
	}
	size_t activePlayerIterator = 0;
	std::reference_wrapper<Player> activePlayer = players[activePlayerIterator];

	// Initialize Board
	Board board;

	// Initialize GUI
	UIGameSession gameSessionGUI(windowSize, pregameSetup, &players, &board, activePlayer);
	logger.Log("Initialized Game GUI", Logger::Level::Info);

	// Load image and create sprite
	cursorTexture.loadFromFile(s_cursorTexture);
	cursorSprite.setTexture(cursorTexture);

	// Game Loop
	gameSessionGUI.StartGame();
	std::cout << "Active player: " << activePlayer.get().GetName() << "\n";
	logger.Log("Game started", Logger::Level::Info);
	while (window->isOpen())
	{
		// Check for Win Condition
		if (CheckForWinCondition(players)) return;

		// Event Handling
		sf::Event event;
		while (window->pollEvent(event))
		{
			gameSessionGUI.PassEvent(event);
			switch (gameSessionGUI.GetEvent())
			{
			case UIGameSession::Events::MenuButton:

				SoundSystem::StopMusic(SoundSystem::MusicType::GameMusic);
				SoundSystem::PlayMusic(SoundSystem::MusicType::MenuMusic);

				return;
			case UIGameSession::Events::PassButton:
				gameSessionGUI.NextTurn();
				++activePlayerIterator;
				if (activePlayerIterator == pregameSetup.GetPlayerCount())
					activePlayerIterator = 0;
				activePlayer = players[activePlayerIterator];
				std::cout << "Active player: " << activePlayer.get().GetName() << "\n";
				break;
			default:
				break;
			}
		}

		// Set sprite position to cursor position
		cursorSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)));

		// Update & Display
		gameSessionGUI.UpdateGame();
		window->clear(UIColors::NavyBlue);
		window->draw(gameSessionGUI);
		window->draw(cursorSprite);
		window->display();
	}
}

void SessionsManager::GameSessionOnline(const PregameSetup& pregameSetup) const
{
	logger.Log("Entered Game Session", Logger::Level::Info);

	// Initialize Database
	CardDAO cardsDatabase;
	logger.Log("Initialized Cards Database", Logger::Level::Info);

	// Stop menu music, then play in-game music
	SoundSystem::StopMusic(SoundSystem::MusicType::MenuMusic);
	SoundSystem::PlayMusic(SoundSystem::MusicType::GameMusic);
	SoundSystem::SetMusicVolume(60);

	// Initialize Players
	std::vector<Player> players;
	for (size_t playerNr = 1; playerNr <= pregameSetup.GetPlayerCount(); ++playerNr)
	{
		players.emplace_back(playerNr, "Player " + std::to_string(playerNr));
	}
	players[0].SetName("Teodor");
	players[1].SetName("Adrian");
	size_t activePlayerIterator = 0;
	std::reference_wrapper<Player> activePlayer = players[activePlayerIterator];

	// Initialize Board
	Board board;

	// Initialize GUI
	UIGameSession gameSessionGUI(windowSize, pregameSetup, &players, &board, activePlayer);
	logger.Log("Initialized Game GUI", Logger::Level::Info);

	// Networking
	Network network;
	NetworkPacket networkPacket;
	bool isSending;

	switch (pregameSetup.GetGameMode())
	{
	case PregameSetup::GameMode::Client:
	{
		isSending = true;
		std::string ipAddress;
		std::cout << "Enter server's IP adress: ";
		std::cin >> ipAddress;
		std::cout << "Connecting to IP " << ipAddress << "...\n";
		network.SetIp(std::move(ipAddress));
		network.InitialiseClient();
		networkPacket.SetDecksData(board.ConvertDecksToPackage());
		networkPacket.SetBoardData(board.ConvertBoardToPackage());
		network.SendData(networkPacket);
		break;
	}
	case PregameSetup::GameMode::Server:
	{
		isSending = false;
		network.InitialiseServer();
		network.AcceptConnection();
		network.ReceiveData(networkPacket);

		// Set-up board & decks
		board.ConvertPackageToBoard(networkPacket);
		gameSessionGUI.SyncBoard();

		// Point to server player
		gameSessionGUI.PointToNextPlayer();
		++activePlayerIterator;
		activePlayer = players[activePlayerIterator];
		break;
	}
	default:
	{
		throw std::invalid_argument("Undefined connection");
	}
	}

	// Load image and create sprite
	cursorTexture.loadFromFile(s_cursorTexture);
	cursorSprite.setTexture(cursorTexture);

	// Game Loop
	gameSessionGUI.StartGame();
	std::cout << "Active player: " << activePlayer.get().GetName() << "\n";
	logger.Log("Game started", Logger::Level::Info);
	while (window->isOpen())
	{
		// Check for Win Condition
		if (CheckForWinCondition(players)) return;

		// Event Handling
		sf::Event event;
		while (window->pollEvent(event))
		{
			gameSessionGUI.PassEvent(event);
			switch (gameSessionGUI.GetEvent())
			{
			case UIGameSession::Events::MenuButton:
			{
				SoundSystem::StopMusic(SoundSystem::MusicType::GameMusic);
				SoundSystem::PlayMusic(SoundSystem::MusicType::MenuMusic);
				return;
			}
			case UIGameSession::Events::PassButton:
			{
				gameSessionGUI.NextTurnOnline();

				// Sending
				if (isSending)
				{
					isSending = false;
					networkPacket.SetHandData(activePlayer.get().GetHand().ConvertToPackage());
					networkPacket.SetBoardData(board.ConvertBoardToPackage());
					networkPacket.SetCardDrawnFromDeck(board.ExtractLastExpansionDrawn());
					networkPacket.SetPlayerData(activePlayer.get().ConvertToPackage());
					network.SendData(networkPacket);
				}
				break;
			}
			default:
			{
				// Recieving
				if (isSending == false)
				{
					isSending = true;
					network.ReceiveData(networkPacket);
					board.ConvertPackageToBoard(networkPacket);
					if (activePlayerIterator == 0)
					{
						players[1].GetHand().ConvertFromPackage(networkPacket);
						players[1].ConvertFromPackage(networkPacket);
						gameSessionGUI.SyncOnlineAdversaryPlayerPanel(players[1].GetPrestigePoints());
					}
					else
					{
						players[0].GetHand().ConvertFromPackage(networkPacket);
						players[0].ConvertFromPackage(networkPacket);
						gameSessionGUI.SyncOnlineAdversaryPlayerPanel(players[0].GetPrestigePoints());
					}

					gameSessionGUI.SyncBoard();
				}
			}

			// Set sprite position to cursor position
			cursorSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)));

			// Update & Display
			gameSessionGUI.UpdateGame();
			window->clear(UIColors::NavyBlue);
			window->draw(gameSessionGUI);
			window->draw(cursorSprite);
			window->display();
			}
		}
	}
}

void SessionsManager::TestSession() const
{
	WinSession("Teodor");
}

void SessionsManager::Leaderboard() const
{

	logger.Log("Entered Leaderboard Session", Logger::Level::Info);
	UILeaderboardSession leaderboardSessionGUI(windowSize);
	logger.Log("Initialized Leaderboard GUI", Logger::Level::Info);

	// Load image and create sprite
	cursorTexture.loadFromFile(s_cursorTexture);
	cursorSprite.setTexture(cursorTexture);

	leaderboardSessionGUI.LoadFromFile();
	leaderboardSessionGUI.LoadLeaderboard(windowSize);

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			leaderboardSessionGUI.PassEvent(event);
			switch (leaderboardSessionGUI.GetEvent())
			{
			case UILeaderboardSession::Events::MainMenu:
				logger.Log("Exiting Tutorial Session...", Logger::Level::Info);
				return;
			default:
				break;
			}
		}



		// Set sprite position to cursor position
		cursorSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)));

		window->clear(UIColors::NavyBlue);
		window->draw(leaderboardSessionGUI);
		window->draw(cursorSprite);
		window->display();
	}
}

void SessionsManager::WinSession(const std::string& winnerName) const
{
	// Text
	UIText winnerText(sf::Vector2f(), UIText::TextAlign::mid_center, UIText::AvailableFonts::LatoBlack, winnerName + " HAS WON THE GAME", 60, UIColors::GoldYellow, UIColors::NeutralWhite, 3);

	winnerText.AlignText(UIText::TextAlign::mid_center);
	winnerText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
	const auto& textBounds = winnerText.getLocalBounds();

	logger.Log(winnerName, Logger::Level::Win);


	// Button
	sf::RectangleShape buttonShape(sf::Vector2f(400, 125));
	buttonShape.setPosition(buttonShape.getPosition().x + textBounds.height, buttonShape.getPosition().y + textBounds.height);
	UIButton menuButton(buttonShape);
	menuButton.ChangeText("Back to Main Menu");

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			menuButton.HandleEvent(event);
			if (menuButton.GetState() == UIButton::State::Release)
			{
				menuButton.SwitchState(UIButton::State::Hover);
				return;
			}
		}

		// Set sprite position to cursor position
		cursorSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)));

		window->clear(UIColors::NavyBlue);
		window->draw(winnerText);
		window->draw(menuButton);
		window->draw(cursorSprite);
		window->display();
	}
}

bool SessionsManager::CheckForWinCondition(const std::vector<Player>& players) const
{
	for (auto& player : players)
	{
		if (player.GetPrestigePoints() >= GamePieces::s_winingPrestigePoints)
		{
			WinSession(player.GetName());
			return true;
		}
	}
	return false;
}
