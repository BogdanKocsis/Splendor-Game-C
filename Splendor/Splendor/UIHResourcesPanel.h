#pragma once
#include <array>
#include <unordered_map>

#include "UIPanel.h"
#include "UIResource.h"
#include "UIText.h"

class UIHResourcesPanel :public UIPanel
{
public:
	UIHResourcesPanel(const sf::Vector2f& position = { 0,0 }, const sf::Vector2f& size = { 100,1024 }, bool isActive = true);

	// Logic
	void UpdateResources(std::unordered_map<IToken::Type, uint16_t>& resources);

private:
	static inline const float s_yPaddingPercentage = 0.02f;
	static inline const float s_xPaddingPercentage = 0.05f;
	static inline const float s_resourcePercentage = 0.14f;

private:
	std::array<UIResource*, 5> m_resources;
	std::array<UIText*, 5> m_resourcesText;
};

