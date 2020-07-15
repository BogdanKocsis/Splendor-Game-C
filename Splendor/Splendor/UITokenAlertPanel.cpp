#include "UITokenAlertPanel.h"
#include "GamePieces.h"
#include "UIColors.h"

UITokenAlertPanel::UITokenAlertPanel(const sf::Vector2f& size, bool isActive) :
	UIPanel("UITokenAlertPanel", size, sf::Vector2f(0, 0), isActive),
	m_cover(size),
	m_background(sf::Vector2f(s_sizeRatio* size.x, s_sizeRatio* size.y)),
	m_alertMessage(sf::Vector2f(), UIText::TextAlign::classic, UIText::AvailableFonts::LatoBlack, "( ! ) Remove exceeding tokens", 30, UIColors::GoldYellow, UIColors::OpaqueBlack),
	m_warningMessage(sf::Vector2f(), UIText::TextAlign::classic, UIText::AvailableFonts::LatoBlack, "ERROR: Tokens still exceed " + std::to_string(GamePieces::s_HeldTokensLimit), 20, UIColors::Transparent, UIColors::Transparent, 0.0f)
{
	// Cover
	m_cover.setFillColor(UIColors::DarkGray - UIColors::QuarterTransparent);

	// Background
	m_background.setOutlineThickness(s_outlineThickness);
	m_background.setOutlineColor(UIColors::GoldYellow);
	m_background.setFillColor(UIColors::NavyBlue);
	m_background.setPosition((size.x - m_background.getSize().x) / 2.0f, (size.y - m_background.getSize().y) / 2.0f);

	// Horizontal Token Panels
	m_tokensToChoose = new UIHTokensPanel(
		sf::Vector2f(m_background.getPosition().x + s_borderPadding * m_background.getSize().x,
			m_background.getPosition().y + (s_borderPadding + s_textSize) * m_background.getSize().y),
		sf::Vector2f(m_background.getSize().x - 2 * s_borderPadding * m_background.getSize().x,
			s_tokensSize * m_background.getSize().y));
	m_tokensChosen = new UIHTokensPanel(
		sf::Vector2f(m_background.getPosition().x + s_borderPadding * m_background.getSize().x,
			m_background.getPosition().y + (s_borderPadding + s_textSize + s_tokensSize) * m_background.getSize().y),
		sf::Vector2f(m_background.getSize().x - 2 * s_borderPadding * m_background.getSize().x,
			s_tokensSize * m_background.getSize().y));
	m_tokensToChoose->UnNumb();
	m_tokensChosen->UnNumb();

	// Position UITexts
	m_alertMessage.setPosition(m_background.getPosition().x + s_borderPadding * m_background.getSize().x,
		m_background.getPosition().y + s_borderPadding * m_background.getSize().y);
	m_warningMessage.setPosition(m_background.getPosition().x + s_borderPadding * m_background.getSize().x,
		m_background.getPosition().y + (s_borderPadding + 2 * s_tokensSize + s_textSize) * m_background.getSize().y + s_padding);

	// UIButtons
	sf::RectangleShape buttonShape(sf::Vector2f(s_buttonWidth, s_buttonsSize * m_background.getSize().y - 2 * s_padding));
	buttonShape.setPosition(m_background.getPosition().x + m_background.getSize().x - s_borderPadding * m_background.getSize().x - s_buttonWidth,
		m_background.getPosition().y + (s_borderPadding + s_textSize * 2 + s_tokensSize * 2) * m_background.getSize().y + 2 * s_padding);
	m_confirmButton = new UIButton(buttonShape);
	m_confirmButton->ChangeText("Confirm");
	buttonShape.setPosition(m_background.getPosition().x + m_background.getSize().x - 2 * s_borderPadding * m_background.getSize().x - 2 * s_buttonWidth,
		m_background.getPosition().y + (s_borderPadding + s_textSize * 2 + s_tokensSize * 2) * m_background.getSize().y + 2 * s_padding);
	m_resetButton = new UIButton(buttonShape);
	m_resetButton->ChangeText("Reset");

	// Add Drawables to UIPanel
	AddContent(dynamic_cast<sf::Drawable*>(&m_cover));
	AddContent(dynamic_cast<sf::Drawable*>(&m_background));
	AddContent(dynamic_cast<sf::Drawable*>(&m_alertMessage));
	AddContent(dynamic_cast<sf::Drawable*>(&m_warningMessage));
	AddContent(dynamic_cast<sf::Drawable*>(m_tokensToChoose));
	AddContent(dynamic_cast<sf::Drawable*>(m_tokensChosen));
	AddContent(dynamic_cast<sf::Drawable*>(m_confirmButton));
	AddContent(dynamic_cast<sf::Drawable*>(m_resetButton));

	// Counters
	m_initialCount = 0;
	m_count = 0;
}

void UITokenAlertPanel::SetInitialTokens(const GemsMap& data)
{
	// Logic
	m_initialTokens = data;
	m_remainingTokens = m_initialTokens;
	m_toReturnTokens.clear();
	InitializeCounters();

	// UI
	m_tokensToChoose->UnNumb();
	m_tokensToChoose->UpdateTokens(m_initialTokens);
	m_tokensChosen->UpdateTokens(m_toReturnTokens);
}

UITokenAlertPanel::GemsMap UITokenAlertPanel::GetTokensToReturn() const
{
	return m_toReturnTokens;
}

void UITokenAlertPanel::HandleEvent(const sf::Event& event)
{
	if (IsActive() && IsInteractable())
	{
		m_tokensChosen->HandleEvent(event);
		m_tokensToChoose->HandleEvent(event);
		m_confirmButton->HandleEvent(event);
		m_resetButton->HandleEvent(event);
	}
}

void UITokenAlertPanel::Update()
{
	// Check for confirmation
	if (m_confirmButton->GetState() == UIButton::State::Release)
	{
		m_confirmButton->SwitchState(UIButton::State::Hover);
		if (IsOk())
		{
			m_confirmed = true;
		}
		else
		{
			// Trigger Warning Text
			m_warningMessage.setFillColor(UIColors::WarningRed);
		}
	}
	// Check for reset
	else if (m_resetButton->GetState() == UIButton::State::Release)
	{
		m_resetButton->SwitchState(UIButton::State::Hover);
		m_tokensToChoose->UnNumb();
		m_remainingTokens = m_initialTokens;
		m_toReturnTokens.clear();
		m_count = m_initialCount;
	}

	{// Update top tokens
		const auto pickedToken = m_tokensToChoose->GetPickedToken();
		if (pickedToken.has_value())
		{
			--m_count;
			--m_remainingTokens[pickedToken.value()];
			++m_toReturnTokens[pickedToken.value()];
		}
		m_tokensToChoose->UpdateTokens(m_remainingTokens);
	}

	{// Update bottom tokens
		const auto pickedToken = m_tokensChosen->GetPickedToken();
		if (pickedToken.has_value())
		{
			m_tokensToChoose->UnNumb();
			++m_count;
			++m_remainingTokens[pickedToken.value()];
			--m_toReturnTokens[pickedToken.value()];
		}
		m_tokensChosen->UpdateTokens(m_toReturnTokens);
	}

	// Check for count
	if (m_count == GamePieces::s_HeldTokensLimit)
	{
		m_tokensToChoose->NumbAll();
		m_warningMessage.setFillColor(UIColors::Transparent);
	}

}

bool UITokenAlertPanel::GetConfirmed() const
{
	return m_confirmed;
}

void UITokenAlertPanel::SetConfirmed(bool confirmed)
{
	m_confirmed = confirmed;
}

void UITokenAlertPanel::InitializeCounters()
{
	m_initialCount = 0;
	for (const auto& token : m_initialTokens)
	{
		m_initialCount += token.second;
	}
	m_count = m_initialCount;
}

bool UITokenAlertPanel::IsOk() const
{
	if (m_count == GamePieces::s_HeldTokensLimit)
		return true;
	return false;
}
