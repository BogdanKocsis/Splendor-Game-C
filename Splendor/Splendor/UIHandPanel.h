#pragma once
#include <array>

// GUI
#include "UIPanel.h"
#include "UIButton.h"
#include "UICardsRowPanel.h"
#include "UIHTokensPanel.h"
#include "UIHResourcesPanel.h"
#include "UIPlayerPanel.h"


class UIHandPanel :public UIPanel
{
public:
	// Constr.
	UIHandPanel(const sf::Vector2f& size = { 1280,720 }, bool isActive = true);

	// G&S
	std::string GetPlayerName() const;
	UICardsRowPanel& GetExpansionsPanel() const;

	// Logic
	void HandleEvent(const sf::Event& event) override;
	std::optional<UICard*> GetPickedExpansion() const;

	// GUI
	void SetUpHand(UIPlayerPanel& playerPanel);
	void SyncHand();
	bool CheckForClose();
	void NumbAllExpansions();

private:
	static inline float s_sizeRatio = 0.75f;// %
	static inline float s_lowPadding = 10;// px
	static inline float s_highPadding = 0.25f;// %
	static inline float s_firstRowRatio = 0.6f;// %
	static inline float s_secondRowRatio = 0.5f;// %
	static inline float s_columnRatio = 0.75f;// %

private:
	// Backgrounds
	sf::RectangleShape m_cover;
	sf::RectangleShape m_outerBackground;
	sf::RectangleShape m_innerBackground;
	std::array<sf::RectangleShape, 4> m_panelBackgrounds;

	// Panels & Buttons
	UIButton* m_closeButton;
	UICardsRowPanel* m_expansionsPanel;
	UICardsRowPanel* m_noblesPanel;
	UIHResourcesPanel* m_resourcesPanel;
	UIHTokensPanel* m_tokensPanel;

	// Profile
	sf::CircleShape m_profile;
	UIText* m_nameLabel;
	UIText* m_prestigeLabel;

	// Info
	sf::Texture* m_profilePicture;

	// Player Panel
	UIPlayerPanel* m_playerPanel;

};

