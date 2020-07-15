#pragma once
#include "UIPanel.h"
#include "UIText.h"
#include "UIButton.h"

#include <chrono>

class UIInfoPanel :public UIPanel
{
public:
	using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

	struct Timer
	{
		bool running = false;
		TimePoint startTime, currentTime;
	};

public:
	UIInfoPanel(const sf::Vector2f& position = { 0,0 }, const sf::Vector2f& size = { 1024,100 }, bool isActive = true);

	// Info manipulation
	void IncrementTurn();
	void UpdateTime();
	void StartTimer();
	void StopTimer();

	// Events
	bool MenuButtonTriggered() const;
	bool PassButtonTriggered() const;

private:
	void UpdateTimeLabel();
	void UpdateTurnLabel();

private:
	static inline const uint16_t s_padding = 10;

private:
	Timer m_timer;
	uint16_t m_turn;
	UIText m_timeTitle, m_timeLabel;
	UIText m_turnTitle, m_turnLabel;
	sf::RectangleShape m_background;
	UIButton* m_menuButton;
	UIButton* m_passButton;
};

