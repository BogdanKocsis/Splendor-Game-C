#include "UIPlayerPanel.h"
#include "Randomizer.h"
#include "UIColors.h"
#include "SoundSystem.h"

#include <sstream>

UIPlayerPanel::UIPlayerPanel(Player* pPlayer, const sf::Vector2f& position, const sf::Vector2f& size, bool isActive) :
	UIPanel("PlayerPanel", size, position, isActive),
	RectCollider(*dynamic_cast<sf::RectangleShape*>(this)),
	m_isTriggered(false),
	// Graphic components
	m_nameLabel(sf::Vector2f(position.x + s_percentage * size.x, position.y + s_padding * size.y), UIText::TextAlign::classic, UIText::AvailableFonts::LatoLight, pPlayer->GetName(), s_fontSize + 10, UIColors::NeutralWhite, UIColors::Transparent),
	m_prestigeLabel(sf::Vector2f(position.x + s_percentage * size.x, m_nameLabel.getGlobalBounds().top + m_nameLabel.getGlobalBounds().height), UIText::TextAlign::classic, UIText::AvailableFonts::LatoBlack, "Prestige Points: 0", s_fontSize - 10, UIColors::NeutralWhite, UIColors::Transparent),
	m_clickToViewLabel(sf::Vector2f(position.x + s_percentage * size.x, m_prestigeLabel.getGlobalBounds().top + m_prestigeLabel.getGlobalBounds().height + s_padding * size.y / 2), UIText::TextAlign::classic, UIText::AvailableFonts::LatoMediumItalic, "(Click to view hand)", s_fontSize - 10, UIColors::NeutralWhite, UIColors::Transparent),
	// Pointer
	p_player(pPlayer)
{
	// Re-scale UIText
	const auto scale = (size.x - 2 * s_padding * size.x) / (m_nameLabel.getGlobalBounds().left + m_nameLabel.getGlobalBounds().width);
	m_nameLabel.setScale(scale, scale);
	m_prestigeLabel.setScale(scale, scale);
	m_clickToViewLabel.setScale(scale, scale);
	// Re-position UIText
	m_nameLabel.setPosition(m_nameLabel.getPosition().x, position.y + s_padding * size.y);
	m_prestigeLabel.setPosition(m_prestigeLabel.getPosition().x, m_nameLabel.getGlobalBounds().top + m_nameLabel.getGlobalBounds().height);
	m_clickToViewLabel.setPosition(m_clickToViewLabel.getPosition().x, position.y + size.y - 2 * s_padding * size.y - m_clickToViewLabel.getGlobalBounds().height);

	// Textures
	if (!s_texturesInitialized)
	{
		LoadIconTextures();
	}

	// Profile
	m_profile.setRadius((size.y - 2 * s_padding * size.y) / 2);
	m_profile.setOrigin(m_profile.getRadius(), m_profile.getRadius());
	m_profile.setPosition(position.x + (size.x * s_percentage) / 2, position.y + size.y / 2);
	if (p_player->GetType() == Player::Type::Computer)
	{
		m_profile.setTexture(s_computerIcon);
	}

	//Background
	m_background.setSize(size);
	m_background.setPosition(position);
	m_background.setFillColor(UIColors::NeutralGray);
	m_background.setOutlineColor(UIColors::GoldYellow);
	m_background.setOutlineThickness(3);

	// Add drawable content to panel
	AddContent(dynamic_cast<Drawable*>(&m_background));
	AddContent(dynamic_cast<Drawable*>(&m_profile));
	AddContent(dynamic_cast<Drawable*>(&m_nameLabel));
	AddContent(dynamic_cast<Drawable*>(&m_prestigeLabel));
	AddContent(dynamic_cast<Drawable*>(&m_clickToViewLabel));
}

bool UIPlayerPanel::GetTriggered() const
{
	return m_isTriggered;
}

void UIPlayerPanel::SetTriggered(bool triggered)
{
	m_isTriggered = triggered;
}

Player* UIPlayerPanel::GetPlayer() const
{
	return p_player;
}

void UIPlayerPanel::OnMouseEnter()
{
	m_background.setFillColor(UIColors::LightGray);
	m_background.setOutlineColor(UIColors::NeutralWhite);
	SoundSystem::PlaySFX(SoundSystem::SoundType::OverSFX);
}

void UIPlayerPanel::OnMouseLeave()
{
	m_background.setFillColor(UIColors::NeutralGray);
	m_background.setOutlineColor(UIColors::GoldYellow);
}

void UIPlayerPanel::OnMouseLeftClick()
{
	m_background.setFillColor(UIColors::DarkGray);
	m_background.setOutlineColor(UIColors::LightGray);
	SoundSystem::PlaySFX(SoundSystem::SoundType::ButtonSFX);
}

void UIPlayerPanel::OnMouseLeftRelease()
{
	m_background.setFillColor(UIColors::LightGray);
	m_background.setOutlineColor(UIColors::NeutralWhite);
	m_isTriggered = true;
}

void UIPlayerPanel::SetUserTexture(uint16_t textureID)
{
	if (p_player->GetType() == Player::Type::User && textureID < 4)
	{
		m_profile.setTexture(s_userIcons[textureID]);
	}
}

sf::Texture UIPlayerPanel::GetUserTexture() const
{
	return *m_profile.getTexture();
}

std::string UIPlayerPanel::GetNameLabelString() const
{
	return m_nameLabel.getString();
}

std::string UIPlayerPanel::GetPrestigeLabelString() const
{
	return m_prestigeLabel.getString();
}

void UIPlayerPanel::AddPrestigePoints(uint16_t prestigePoints)
{
	std::stringstream sStream(m_prestigeLabel.getString());
	std::string firstPart, secondPart;
	sStream >> firstPart;
	sStream >> secondPart;
	firstPart = firstPart + " " + secondPart;
	sStream >> secondPart;
	uint16_t prestige = std::stoi(secondPart);
	prestige += prestigePoints;
	secondPart = std::to_string(prestige);
	m_prestigeLabel.setString(firstPart + " " + secondPart);
}

void UIPlayerPanel::SetPrestigePoints(uint16_t prestigePoints)
{
	std::stringstream sStream(m_prestigeLabel.getString());
	std::string firstPart, secondPart;
	sStream >> firstPart;
	sStream >> secondPart;
	firstPart = firstPart + " " + secondPart;
	sStream >> secondPart;
	secondPart = std::to_string(prestigePoints);
	m_prestigeLabel.setString(firstPart + " " + secondPart);
}

void UIPlayerPanel::ShuffleTextures()
{
	Randomizer randomizer;
	randomizer.Shuffle(s_userIcons);
}

void UIPlayerPanel::LoadIconTextures()
{
	s_computerIcon = new sf::Texture;
	s_computerIcon->loadFromFile(s_iconsTextureFile + "/computer.png");
	char textureID = '1';
	std::for_each(s_userIcons.begin(), s_userIcons.end(), [&textureID](sf::Texture*& texture)
		{
			texture = new sf::Texture;
			texture->loadFromFile(s_iconsTextureFile + "/user" + textureID + ".png");
			++textureID;
		});
	s_texturesInitialized = true;
}
