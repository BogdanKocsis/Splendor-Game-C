#include "UISelector.h"

UISelector::UISelector(const std::string& title, const sf::Vector2f& position, const sf::Vector2f& size, bool initialState) :
	Rect<float>(position, size),
	m_title(UIText(position, UIText::TextAlign::mid_center, UIText::AvailableFonts::LatoBlack, title, size.y * 0.2f)),
	m_isChecked(initialState)
{
	m_title.setPosition(position.x + size.x / 2.0f, (position.y + 0.3f * size.y / 2.0f));
	m_title.AlignText(UIText::TextAlign::mid_center);
}

std::string UISelector::GetName()
{
	return m_title.getString();
}

bool UISelector::GetState() const
{
	return m_isChecked;
}

void UISelector::SetState(bool state)
{
	m_isChecked = state;
}

bool UISelector::IsChecked() const
{
	return m_isChecked;
}
