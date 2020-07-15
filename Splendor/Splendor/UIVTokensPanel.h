#pragma once
#include <array>
#include <optional>
#include <unordered_map>

#include "UIPanel.h"
#include "UIToken.h"
#include "UIText.h"


class UIVTokensPanel :public UIPanel
{
public:
	using PickedArray = std::array<std::optional<IToken::Type>, 3>;

public:
	UIVTokensPanel(const sf::Vector2f& position = { 0,0 }, const sf::Vector2f& size = { 100,1024 }, bool isActive = true);

	// Logic
	void UpdateTokens(std::unordered_map<IToken::Type, uint16_t>&& boardTokens);
	void SyncTokens(std::unordered_map<IToken::Type, uint16_t>&& boardTokens);
	PickedArray& ExtractPickedTokens();
	void NumbAll();
	void UnNumb();
	bool IsNumb();
	std::optional<IToken::Type>& GetLastPicked();
	void TakeGoldToken();

	// G&S
	bool GetHasPicked() const;
	void SetHasPicked(bool hasPicked);

private:
	// picked tokens array
	bool AddPickedToken(IToken::Type tokenType, uint16_t availableTokens);

private:
	static inline const float s_xPaddingPercentage = 0.02f;
	static inline const float s_yPaddingPercentage = 0.04f;
	static inline const float s_tokenPercentage = 0.12f;

private:
	std::array<UIToken*, 6> m_tokens;
	std::array<UIText*, 6> m_tokensText;
	PickedArray m_pickedTokens;
	bool m_hasPicked;
	std::optional<IToken::Type> m_lastPicked;

};

