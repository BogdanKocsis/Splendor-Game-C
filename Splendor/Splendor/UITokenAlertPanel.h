#pragma once
#include "IToken.h"
#include "UIPanel.h"
#include "UIHTokensPanel.h"
#include "UIButton.h"
#include "UIText.h"

#include <unordered_map>

class UITokenAlertPanel :public UIPanel
{
public:
	using GemsMap = std::unordered_map<IToken::Type, uint16_t>;

public:
	// Constr.
	UITokenAlertPanel(const sf::Vector2f& size = { 1280,720 }, bool isActive = true);

	// G&S
	void SetInitialTokens(const GemsMap& data);
	GemsMap GetTokensToReturn() const;

	// Logic
	void HandleEvent(const sf::Event& event) override;
	void Update();
	bool GetConfirmed() const;
	void SetConfirmed(bool confirmed);

private:
	void InitializeCounters();
	bool IsOk() const;

private:
	static inline float s_sizeRatio = 0.5f;// %
	static inline float s_textSize = 0.1f;// %
	static inline float s_buttonsSize = 0.2f;// %
	static inline float s_tokensSize = 0.25f;// %
	static inline float s_borderPadding = 0.05f;// %
	static inline uint16_t s_padding = 5;// px
	static inline uint16_t s_outlineThickness = 5;// px
	static inline float s_buttonWidth = 250.0f;// px

private:
	// GUI
	sf::RectangleShape m_cover;
	sf::RectangleShape m_background;
	UIText m_alertMessage;
	UIText m_warningMessage;
	UIButton* m_confirmButton;
	UIButton* m_resetButton;
	UIHTokensPanel* m_tokensToChoose;
	UIHTokensPanel* m_tokensChosen;

	// Logic
	bool m_confirmed;
	GemsMap m_initialTokens;
	GemsMap m_remainingTokens;
	GemsMap m_toReturnTokens;
	uint16_t m_initialCount;
	uint16_t m_count;

};

