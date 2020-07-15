#pragma once
#include <array>
#include <optional>
#include <unordered_map>

#include "UIPanel.h"
#include "UIToken.h"
#include "UIText.h"

class UIHTokensPanel :public UIPanel
{
public:
	UIHTokensPanel(const sf::Vector2f& position = { 0,0 }, const sf::Vector2f& size = { 100,1024 }, bool isActive = true);

	// Logic
	void UpdateTokens(std::unordered_map<IToken::Type, uint16_t>& tokens);
	void ResetTokens();
	void NumbAll();
	void UnNumb();
	std::optional<IToken::Type> GetPickedToken();

private:
	static inline const float s_yPaddingPercentage = 0.02f;
	static inline const float s_xPaddingPercentage = 0.04f;
	static inline const float s_tokenPercentage = 0.12f;

private:
	// GUI
	std::array<UIToken*, 6> m_tokens;
	std::array<UIText*, 6> m_tokensText;

};

