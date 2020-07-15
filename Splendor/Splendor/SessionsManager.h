#pragma once
#include <SFML/Graphics.hpp>
#include "../Logging/Logger.h"

#include <fstream>

class PregameSetup;
class Player;

class SessionsManager
{
public:
	SessionsManager();
	~SessionsManager();

	void MainMenuSession() const;
	void PreGameSession() const;
	void TutorialSession() const;
	void GameSessionOffline(const PregameSetup& pregameSetup) const;
	void GameSessionOnline(const PregameSetup& pregameSetup) const;
	void TestSession() const;
	void Leaderboard() const;
	void SettingsSession() const;
	void WinSession(const std::string& winnerName) const;

private:
	bool CheckForWinCondition(const std::vector<Player>& players) const;

private:
	static inline const std::string s_logFile = "../Logging/LogFileStream.log";
	static inline const std::string s_cursorTexture = "../external/Resources/Textures/UI/cursor.png";

private:
	// Window
	sf::RenderWindow* window;
	sf::Vector2i windowPosition;
	sf::Vector2u windowSize;

	// Logger
	Logger logger;
	std::ofstream logFileStream;

	// Cursor
	mutable sf::Texture cursorTexture;
	mutable sf::Sprite cursorSprite;
};