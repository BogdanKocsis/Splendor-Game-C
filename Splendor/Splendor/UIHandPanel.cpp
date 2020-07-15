#include "UIHandPanel.h"
#include "UIColors.h"

#include "Player.h"// Logic

UIHandPanel::UIHandPanel(const sf::Vector2f& size, bool isActive) :
	UIPanel("UIHandPanel", size, sf::Vector2f(0, 0), isActive),
	m_cover(size),
	m_outerBackground(sf::Vector2f(size.x* s_sizeRatio, size.y* s_sizeRatio)),
	m_innerBackground(sf::Vector2f(size.x* s_sizeRatio - 2 * s_lowPadding, size.y* s_sizeRatio - s_highPadding * size.y * s_sizeRatio - s_lowPadding)),
	m_panelBackgrounds({
		sf::RectangleShape(sf::Vector2f(m_innerBackground.getSize().x - 2 * s_lowPadding - (1 - s_firstRowRatio) * m_innerBackground.getSize().x,m_innerBackground.getSize().y - s_lowPadding - (1 - s_columnRatio) * m_innerBackground.getSize().y)),
		sf::RectangleShape(sf::Vector2f(m_innerBackground.getSize().x - s_lowPadding - s_firstRowRatio * m_innerBackground.getSize().x,m_innerBackground.getSize().y - s_lowPadding - (1 - s_columnRatio) * m_innerBackground.getSize().y)),
		sf::RectangleShape(sf::Vector2f(m_innerBackground.getSize().x - 2 * s_lowPadding - (1 - s_secondRowRatio) * m_innerBackground.getSize().x,m_innerBackground.getSize().y - 2 * s_lowPadding - s_columnRatio * m_innerBackground.getSize().y)),
		sf::RectangleShape(sf::Vector2f(m_innerBackground.getSize().x - s_lowPadding - s_secondRowRatio * m_innerBackground.getSize().x,m_innerBackground.getSize().y - 2 * s_lowPadding - s_columnRatio * m_innerBackground.getSize().y))
		})
{
	// Cover
	m_cover.setPosition(sf::Vector2f());
	m_cover.setFillColor(UIColors::DarkGray - UIColors::QuarterTransparent);

	// Outer Background
	m_outerBackground.setOrigin(m_outerBackground.getSize().x / 2, m_outerBackground.getSize().y / 2);
	m_outerBackground.setPosition(size.x / 2, size.y / 2);
	m_outerBackground.setFillColor(UIColors::GoldYellow);

	// Inner Background
	m_innerBackground.setOrigin(m_innerBackground.getSize().x / 2, m_innerBackground.getSize().y);
	m_innerBackground.setPosition(size.x / 2, size.y / 2 + m_outerBackground.getSize().y / 2 - s_lowPadding);
	m_innerBackground.setFillColor(UIColors::DarkBlue);

	// Panel Backgrounds
	for (auto& panel : m_panelBackgrounds)
	{
		panel.setFillColor(UIColors::NavyBlue);
	}
	const auto leftTopCorner = sf::Vector2f(m_innerBackground.getPosition().x - m_innerBackground.getSize().x / 2, m_innerBackground.getPosition().y - m_innerBackground.getSize().y);
	m_panelBackgrounds[0].setPosition(leftTopCorner.x + s_lowPadding, leftTopCorner.y + s_lowPadding);
	m_panelBackgrounds[1].setPosition(leftTopCorner.x + 2 * s_lowPadding + m_panelBackgrounds[0].getSize().x, leftTopCorner.y + s_lowPadding);
	m_panelBackgrounds[2].setPosition(leftTopCorner.x + s_lowPadding, leftTopCorner.y + 2 * s_lowPadding + m_panelBackgrounds[0].getSize().y);
	m_panelBackgrounds[3].setPosition(leftTopCorner.x + 2 * s_lowPadding + m_panelBackgrounds[2].getSize().x, leftTopCorner.y + 2 * s_lowPadding + m_panelBackgrounds[0].getSize().y);

	// Panels
	m_expansionsPanel = new UICardsRowPanel(3, m_panelBackgrounds[0].getPosition(), m_panelBackgrounds[0].getSize(), sf::Vector2f(0.025f, 0.075f));
	m_expansionsPanel->NumbAll();
	m_noblesPanel = new UICardsRowPanel(5, m_panelBackgrounds[1].getPosition(), m_panelBackgrounds[1].getSize(), sf::Vector2f(0.025f, 0.075f));
	m_resourcesPanel = new UIHResourcesPanel(m_panelBackgrounds[2].getPosition(), m_panelBackgrounds[2].getSize());
	m_tokensPanel = new UIHTokensPanel(m_panelBackgrounds[3].getPosition(), m_panelBackgrounds[3].getSize());

	// Close Button
	sf::RectangleShape buttonShape;
	UIButton::Design buttonNoneDesign(
		{
			UIColors::DarkBlue,
			UIColors::NeutralWhite,
			3.0f
		},
		{
			"",
			UIText::AvailableFonts::DosisLight,
			30,
			UIColors::NeutralWhite,
			UIColors::NeutralWhite,
			1.5f
		});
	const float buttonSize = (m_outerBackground.getSize().y - m_innerBackground.getSize().y - 3 * s_lowPadding) / 2.0f;
	buttonShape.setSize(sf::Vector2f(buttonSize, buttonSize));
	buttonShape.setPosition(m_outerBackground.getPosition().x + m_outerBackground.getSize().x / 2.0f - buttonSize - s_lowPadding - 3.0f,
		m_outerBackground.getPosition().y - m_outerBackground.getSize().y / 2.0f + s_lowPadding + 3.0f);
	m_closeButton = new UIButton(buttonShape, buttonNoneDesign);
	m_closeButton->ChangeText("x");

	// Profile - Image
	m_profile.setRadius((m_outerBackground.getSize().y - m_innerBackground.getSize().y - 5 * s_lowPadding) / 2.0f);
	m_profile.setPosition(m_outerBackground.getPosition().x - m_outerBackground.getSize().x / 2.0f + 2 * s_lowPadding,
		m_outerBackground.getPosition().y - m_outerBackground.getSize().y / 2.0f + 2 * s_lowPadding);

	// Profile - Labels
	const float yProfileCenter = m_profile.getPosition().y + m_profile.getRadius();
	m_nameLabel = new UIText(sf::Vector2f(m_profile.getPosition().x + m_profile.getRadius() * 2 + s_lowPadding * 2,
		0), UIText::TextAlign::classic, UIText::AvailableFonts::LatoLight, "Player Name", size.x / 48, UIColors::DarkBlue, UIColors::Transparent);
	m_prestigeLabel = new UIText(sf::Vector2f(m_profile.getPosition().x + m_profile.getRadius() * 2 + s_lowPadding * 2,
		yProfileCenter + s_lowPadding), UIText::TextAlign::classic, UIText::AvailableFonts::LatoBlack, "Prestige Points: 0", size.x / 48, UIColors::DarkBlue, UIColors::Transparent);
	m_nameLabel->setPosition(m_nameLabel->getPosition().x, yProfileCenter - m_nameLabel->getGlobalBounds().height - s_lowPadding);

	// Add content to UIPanel
	AddContent(dynamic_cast<sf::Drawable*>(&m_cover));
	AddContent(dynamic_cast<sf::Drawable*>(&m_outerBackground));
	AddContent(dynamic_cast<sf::Drawable*>(&m_innerBackground));
	for (auto& panel : m_panelBackgrounds)
	{
		AddContent(dynamic_cast<sf::Drawable*>(&panel));
	}
	AddContent(dynamic_cast<sf::Drawable*>(m_expansionsPanel));
	AddContent(dynamic_cast<sf::Drawable*>(m_noblesPanel));
	AddContent(dynamic_cast<sf::Drawable*>(m_resourcesPanel));
	AddContent(dynamic_cast<sf::Drawable*>(m_tokensPanel));
	AddContent(dynamic_cast<sf::Drawable*>(m_closeButton));
	AddContent(dynamic_cast<sf::Drawable*>(&m_profile));
	AddContent(dynamic_cast<sf::Drawable*>(m_nameLabel));
	AddContent(dynamic_cast<sf::Drawable*>(m_prestigeLabel));

}

std::string UIHandPanel::GetPlayerName() const
{
	return m_nameLabel->getString();
}

UICardsRowPanel& UIHandPanel::GetExpansionsPanel() const
{
	return *m_expansionsPanel;
}

void UIHandPanel::HandleEvent(const sf::Event& event)
{
	if (IsActive())
	{
		m_closeButton->HandleEvent(event);
		if (IsInteractable())
		{
			m_expansionsPanel->HandleEvent(event);
		}
	}
}

std::optional<UICard*> UIHandPanel::GetPickedExpansion() const
{
	const auto pickedCard = m_expansionsPanel->CheckForPickedCard();
	if (!pickedCard.has_value()) return std::nullopt;
	return std::optional<UICard*>(pickedCard.value().first);
}

void UIHandPanel::SetUpHand(UIPlayerPanel& playerPanel)
{
	// Player Panel
	m_playerPanel = &playerPanel;

	// Stats
	m_profile.setTexture(new sf::Texture(playerPanel.GetUserTexture()));
	m_nameLabel->setString(playerPanel.GetNameLabelString());
	m_prestigeLabel->setString(playerPanel.GetPrestigeLabelString());

	// Hand
	const Hand playerHand = playerPanel.GetPlayer()->GetHand();
	m_noblesPanel->SetCardsData(playerHand.GetNoblesData());
	m_expansionsPanel->SetCardsData(playerHand.GetExpansionsData());
	m_expansionsPanel->InHandAll();
	m_expansionsPanel->UnNumbAll();
	m_tokensPanel->UpdateTokens(playerHand.GetTokensData());
	m_resourcesPanel->UpdateResources(playerHand.GetResourcesData());
}

void UIHandPanel::SyncHand()
{
	SetUpHand(*m_playerPanel);
}

bool UIHandPanel::CheckForClose()
{
	if (m_closeButton->GetState() == UIButton::State::Release)
	{
		SetActive(false);
		m_closeButton->SwitchState(UIButton::State::None);
		return true;
	}
	return false;
}

void UIHandPanel::NumbAllExpansions()
{
	m_expansionsPanel->NumbAll();
}
