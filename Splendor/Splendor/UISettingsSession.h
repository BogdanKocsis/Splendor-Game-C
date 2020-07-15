#pragma once

// UI
#include "UIText.h"
#include "UIButton.h"
#include "UIOptionsPanel.h"
#include "UISelectorBox.h"
#include "Collider.h"
#include "UIColors.h"
#include "SoundSystem.h"


class UISettingsSession :public sf::Drawable
{
public:
	enum class Events
	{
		None,
		MainMenu

	};

public:
	UISettingsSession(const sf::Vector2u& windowSize);
	~UISettingsSession();

	// Events
	void PassEvent(const sf::Event& event);
	Events GetEvent() const;

	// Graphics
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const override;

	//Update
	void UpdateSound();
	static inline  bool activeSound=true;
	static inline  bool activeSFX=true;
	static inline bool GetActiveSound();
	static inline bool GetActiveSFX();

private:
	UIText m_title;
	sf::RectangleShape m_titleBackground;
	UIOptionsPanel m_musicPanel;
	UIOptionsPanel m_sfxPanel;
	UIButton* m_mainMenuButton;


};

