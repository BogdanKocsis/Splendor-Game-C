#pragma once
// UI
#include "UIText.h"
#include "UIButton.h"
#include "UIOptionsPanel.h"
#include "UISelectorBox.h"
#include "Collider.h"
#include "UIColors.h"

// Logic
#include "PregameSetup.h"

class UIPreGameSession :public sf::Drawable
{
public:
	enum class Events
	{
		None,
		MainMenu,
		StartGame
	};

public:
	UIPreGameSession(const sf::Vector2u& windowSize);
	~UIPreGameSession();

	// Events
	void PassEvent(const sf::Event& event);
	Events GetEvent() const;

	// Graphics
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const override;

	// Logic
	PregameSetup GetPregameSetup() const;

private:
	UIText m_title;
	sf::RectangleShape m_titleBackground;
	UIOptionsPanel m_gameModePanel, m_playersPanel, m_otherSettingsPanel;
	UIButton* m_startGameButton;
	UIButton* m_mainMenuButton;

};

