#include "UIVTokensPanel.h"
#include "GamePieces.h"

UIVTokensPanel::UIVTokensPanel(const sf::Vector2f& position, const sf::Vector2f& size, bool isActive) :
	UIPanel("UIVTokensPanel", size, position, isActive)
{
	const auto tokenRadius = s_tokenPercentage * size.y / 2;
	for (uint16_t tokenIt = 0; tokenIt < IToken::s_typeCount; ++tokenIt)
	{
		// UIToken
		m_tokens[tokenIt] = new UIToken(static_cast<IToken::Type>(tokenIt), sf::Vector2f(0, 0), tokenRadius);
		m_tokens[tokenIt]->SetOrigin(tokenRadius, tokenRadius);
		m_tokens[tokenIt]->SetPosition(position.x + size.x / 2, position.y + tokenRadius + (tokenIt + 1) * s_yPaddingPercentage * size.y + tokenIt * 2 * tokenRadius);
		auto tokenBody = m_tokens[tokenIt]->GetBody();
		AddContent(dynamic_cast<sf::Drawable*>(m_tokens[tokenIt]));
		AddContent(dynamic_cast<Collider*>(m_tokens[tokenIt]));

		// UIText
		m_tokensText[tokenIt] = new UIText(sf::Vector2f(tokenBody.getPosition().x + tokenRadius, tokenBody.getPosition().y + tokenRadius), UIText::TextAlign::classic, UIText::AvailableFonts::BerlinBold, std::to_string(GamePieces::GetGemTokenCount()), 60);
		m_tokensText[tokenIt]->AlignText(UIText::TextAlign::mid_center);
		m_tokensText[tokenIt]->setPosition(sf::Vector2f(tokenBody.getPosition().x + tokenRadius - 6 * s_xPaddingPercentage * size.x, tokenBody.getPosition().y + tokenRadius - 6 * s_xPaddingPercentage * size.x));
		AddContent(dynamic_cast<sf::Drawable*>(m_tokensText[tokenIt]));
	}
	m_tokens[5]->SetNumb(true);
	m_tokensText[5]->setString(std::to_string(GamePieces::s_GoldTokenCount));
}

void UIVTokensPanel::UpdateTokens(std::unordered_map<IToken::Type, uint16_t>&& boardTokens)
{
	uint16_t it = 0;
	for (const auto& token : m_tokens)
	{
		if (token->GetState() == UIToken::State::Release)
		{
			token->SetState(UIToken::State::Hover);
			// Add picked token
			if (boardTokens[token->GetType()] != 0)
			{
				if (AddPickedToken(token->GetType(), boardTokens[token->GetType()]))
				{
					// Update lastPicked
					m_lastPicked.emplace(token->GetType());

					// Update text
					std::string newString(m_tokensText[it]->getString());
					newString[0]--;
					m_tokensText[it]->setString(newString);
				}
			}
		}
		++it;
	}
}

void UIVTokensPanel::SyncTokens(std::unordered_map<IToken::Type, uint16_t>&& boardTokens)
{
	for (uint16_t gemTypeIterator = 0; gemTypeIterator < IToken::s_typeCount; ++gemTypeIterator)
	{
		const auto gemType = static_cast<IToken::Type>(gemTypeIterator);
		m_tokensText[gemTypeIterator]->setString(std::to_string(boardTokens[gemType]));
	}
}

UIVTokensPanel::PickedArray& UIVTokensPanel::ExtractPickedTokens()
{
	return m_pickedTokens;
}

void UIVTokensPanel::NumbAll()
{
	for (auto& token : m_tokens)
	{
		token->SetNumb(true);
	}
}

void UIVTokensPanel::UnNumb()
{
	for (auto& token : m_tokens)
	{
		token->SetNumb(false);
	}
	m_tokens[5]->SetNumb(true);
}

bool UIVTokensPanel::IsNumb()
{
	return m_tokens[0]->GetNumb();
}

std::optional<IToken::Type>& UIVTokensPanel::GetLastPicked()
{
	return m_lastPicked;
}

void UIVTokensPanel::TakeGoldToken()
{
	// Update text
	std::string newString(m_tokensText[IToken::s_typeCount - 1]->getString());
	newString[0]--;
	m_tokensText[IToken::s_typeCount - 1]->setString(newString);
}

bool UIVTokensPanel::GetHasPicked() const
{
	return m_hasPicked;
}

void UIVTokensPanel::SetHasPicked(bool hasPicked)
{
	m_hasPicked = hasPicked;
}

bool UIVTokensPanel::AddPickedToken(IToken::Type tokenType, uint16_t availableTokens)
{
	if (m_pickedTokens[2].has_value())// all slots full
	{
		return false;
	}
	if (!m_pickedTokens[0].has_value())// all slots empty
	{
		m_hasPicked = true;
		m_pickedTokens[0] = tokenType;
		return true;
	}
	if (!m_pickedTokens[1].has_value())// second slot empty
	{
		if (tokenType == m_pickedTokens[0] && availableTokens < 3)// pick only if there are 4+ tokens of the same type availible
			return false;
		m_pickedTokens[1] = tokenType;
		return true;
	}// third slot empty
	if (tokenType != m_pickedTokens[0] && tokenType != m_pickedTokens[1])
	{
		if (m_pickedTokens[0] != m_pickedTokens[1])
		{
			m_pickedTokens[2] = tokenType;
			return true;
		}
	}
	return false;
}
