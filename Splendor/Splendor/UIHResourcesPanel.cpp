#include "UIHResourcesPanel.h"

UIHResourcesPanel::UIHResourcesPanel(const sf::Vector2f& position, const sf::Vector2f& size, bool isActive) :
	UIPanel("UIHResourcesPanel", size, position, isActive)
{
	const auto resourceRadius = s_resourcePercentage * size.x / 2;
	for (uint16_t tokenIt = 0; tokenIt < 5; ++tokenIt)
	{
		// UIToken
		m_resources[tokenIt] = new UIResource(static_cast<IToken::Type>(tokenIt), sf::Vector2f(0, 0), resourceRadius);
		m_resources[tokenIt]->setOrigin(resourceRadius, resourceRadius);
		m_resources[tokenIt]->setPosition(position.x + resourceRadius + (tokenIt + 1) * s_xPaddingPercentage * size.x + tokenIt * 2 * resourceRadius, position.y + size.y / 2);
		sf::CircleShape* resourceBody = dynamic_cast<sf::CircleShape*>(m_resources[tokenIt]);
		AddContent(dynamic_cast<sf::Drawable*>(m_resources[tokenIt]));

		// UIText
		m_resourcesText[tokenIt] = new UIText(sf::Vector2f(resourceBody->getPosition().x + resourceRadius, resourceBody->getPosition().y + resourceRadius), UIText::TextAlign::classic, UIText::AvailableFonts::BerlinBold, "0", 60);
		m_resourcesText[tokenIt]->AlignText(UIText::TextAlign::mid_center);
		m_resourcesText[tokenIt]->setPosition(sf::Vector2f(resourceBody->getPosition().x + resourceRadius / 1.5, resourceBody->getPosition().y + resourceRadius / 1.5));
		AddContent(dynamic_cast<sf::Drawable*>(m_resourcesText[tokenIt]));
	}
}

void UIHResourcesPanel::UpdateResources(std::unordered_map<IToken::Type, uint16_t>& resources)
{
	for (size_t resourceTextIndex = 0; resourceTextIndex < m_resourcesText.size(); ++resourceTextIndex)
	{
		m_resourcesText[resourceTextIndex]->setString(std::to_string(resources[m_resources[resourceTextIndex]->GetType()]));
	}
}
