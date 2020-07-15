#include "UISelectorBox.h"
#include "SoundSystem.h"

UISelectorBox::UISelectorBox(const std::string& title, Type type, const sf::Vector2f& position, const sf::Vector2f& size, const Design& uncheckedDesign, const Design& checkedDesign) :
	UISelector(title, position, size, false),
	RectCollider(
		sf::Vector2i(position.x + (size.x / 2.0f - 0.15f * size.y),
			position.y + (0.6f * size.y / 2.0f + 0.15f * size.y)),
		sf::Vector2i(0.3f * size.y, 0.3f * size.y)),
	m_uncheckedDesign(uncheckedDesign),
	m_checkedDesign(checkedDesign),
	m_type(type)
{
	m_box.setPosition(m_body.left, m_body.top);
	m_box.setSize(sf::Vector2f(m_body.width, m_body.height));
	m_box.setOutlineThickness(m_box.getSize().x / 8.0f);
	m_currentDesign = &m_uncheckedDesign;
	
	// CheckBox Mark Sprite
	m_checkTexture = new sf::Texture();
	switch (m_type)
	{
	case Type::Check:
		m_checkTexture->loadFromFile(s_checkMarkFilePath);
		break;
	case Type::Radio:
		m_checkTexture->loadFromFile(s_radioMarkFilePath);
		break;
	default:
		m_checkTexture->loadFromFile(s_checkMarkFilePath);
		break;
	}
	m_checkSprite.setTexture(*m_checkTexture);
	m_checkSprite.setOrigin(m_checkTexture->getSize().x / 2.0f, m_checkTexture->getSize().y / 2.0f);
	const float xSpriteRatio = m_box.getSize().x / m_checkTexture->getSize().x;
	const float ySpriteRatio = m_box.getSize().y / m_checkTexture->getSize().y;
	m_checkSprite.setScale(xSpriteRatio - 0.02f, ySpriteRatio - 0.02f);
	m_checkSprite.setPosition(m_box.getPosition().x + m_box.getSize().x / 2.0f, m_box.getPosition().y + m_box.getSize().y / 2.0f);
	UpdateDesign();
}

void UISelectorBox::SetState(bool state)
{
	m_isChecked = state;
	if (state)
	{
		m_currentDesign = &m_checkedDesign;
	}
	else
	{
		m_currentDesign = &m_uncheckedDesign;
	}
	UpdateDesign();
}

void UISelectorBox::ChangeState()
{
	if (m_isChecked)
	{
		m_isChecked = false;
		m_currentDesign = &m_uncheckedDesign;
	}
	else
	{
		m_isChecked = true;
		m_currentDesign = &m_checkedDesign;
	}
	UpdateDesign();
}

void UISelectorBox::OnMouseLeftClick()
{
	ChangeState();
	SoundSystem::PlaySFX(SoundSystem::SoundType::CheckBoxSFX);
}

void UISelectorBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_title);
	target.draw(m_box);
	target.draw(m_checkSprite);
}

void UISelectorBox::UpdateDesign()
{
	m_box.setFillColor(m_currentDesign->fillColor);
	m_box.setOutlineColor(m_currentDesign->outlineColor);
	if (m_isChecked)
	{
		m_checkSprite.setColor(UIColors::OpaqueWhite);
	}
	else
	{
		m_checkSprite.setColor(UIColors::Transparent);
	}
}
