#include "UIHTokensPanel.h"

UIHTokensPanel::UIHTokensPanel(const sf::Vector2f& position, const sf::Vector2f& size, bool isActive) :
	UIPanel("UIHTokensPanel", size, position, isActive)
{
	const auto tokenRadius = s_tokenPercentage * size.x / 2;
	for (uint16_t tokenIt = 0; tokenIt < IToken::s_typeCount; ++tokenIt)
	{
		// UIToken
		m_tokens[tokenIt] = new UIToken(static_cast<IToken::Type>(tokenIt), sf::Vector2f(0, 0), tokenRadius);
		m_tokens[tokenIt]->SetOrigin(tokenRadius, tokenRadius);
		m_tokens[tokenIt]->SetPosition(position.x + tokenRadius + (tokenIt + 1) * s_xPaddingPercentage * size.x + tokenIt * 2 * tokenRadius, position.y + size.y / 2);
		m_tokens[tokenIt]->SetNumb(true);
		auto tokenBody = m_tokens[tokenIt]->GetBody();
		AddContent(dynamic_cast<sf::Drawable*>(m_tokens[tokenIt]));
		AddContent(dynamic_cast<Collider*>(m_tokens[tokenIt]));

		// UIText
		m_tokensText[tokenIt] = new UIText(sf::Vector2f(tokenBody.getPosition().x + tokenRadius, tokenBody.getPosition().y + tokenRadius), UIText::TextAlign::classic, UIText::AvailableFonts::BerlinBold, "0", 60);
		m_tokensText[tokenIt]->AlignText(UIText::TextAlign::mid_center);
		m_tokensText[tokenIt]->setPosition(sf::Vector2f(tokenBody.getPosition().x + tokenRadius / 1.5, tokenBody.getPosition().y + tokenRadius / 1.5));
		AddContent(dynamic_cast<sf::Drawable*>(m_tokensText[tokenIt]));
	}
}

void UIHTokensPanel::UpdateTokens(std::unordered_map<IToken::Type, uint16_t>& tokens)
{
	for (size_t tokenTextIndex = 0; tokenTextIndex < m_tokensText.size(); ++tokenTextIndex)
	{
		m_tokensText[tokenTextIndex]->setString(std::to_string(tokens[m_tokens[tokenTextIndex]->GetType()]));
	}
}

void UIHTokensPanel::ResetTokens()
{
	for (size_t tokenTextIndex = 0; tokenTextIndex < m_tokensText.size(); ++tokenTextIndex)
	{
		m_tokensText[tokenTextIndex]->setString("0");
	}
}

void UIHTokensPanel::NumbAll()
{
	for (auto& token : m_tokens)
	{
		token->SetNumb(true);
	}
}

void UIHTokensPanel::UnNumb()
{
	for (auto& token : m_tokens)
	{
		token->SetNumb(false);
	}
}

std::optional<IToken::Type> UIHTokensPanel::GetPickedToken()
{
	std::optional<IToken::Type> pickedToken;

	size_t it = 0;
	for (auto& token : m_tokens)
	{
		if (token->GetState() == UIToken::State::Release)
		{
			token->SetState(UIToken::State::Hover);
			if (m_tokensText[it]->getString()[0] != '0')
				pickedToken.emplace(token->GetType());
		}
		++it;
	}

	return pickedToken;
}
