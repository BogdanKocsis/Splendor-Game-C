#include "UIButton.h"
#include "UIColors.h"
#include "SoundSystem.h"

UIButton::Design::Design(const Body& bodyDesign, const Text& textDesign) : m_bodyDesign(bodyDesign), m_textDesign(textDesign) {}

UIButton::Design::Design(Body&& bodyDesign, Text&& textDesign) : m_bodyDesign(bodyDesign), m_textDesign(std::move(textDesign)) {}

void UIButton::Design::SetBodyDesign(const Body& bodyDesign)
{
	m_bodyDesign = bodyDesign;
}

void UIButton::Design::SetTextDesign(const Text& textDesign)
{
	m_textDesign = textDesign;
}

const UIButton::Design UIButton::DefaultDesign::NoneDesign = UIButton::Design
{
	Design::Body
	{
	sf::Color(0, 0, 0,0),
	UIColors::GoldYellow,
	4.0f
	},
	Design::Text
	{
	"Button",
	UIText::AvailableFonts::DosisBold,
	30,
	UIColors::GoldYellow,
	sf::Color(0,0,0),
	0.0f
	}
};
const UIButton::Design UIButton::DefaultDesign::HoverDesign = UIButton::Design
{
	Design::Body
	{
	UIColors::GoldYellow,
	sf::Color(255,255,255),
	2.0f
	},
	Design::Text
	{
	"Button",
	UIText::AvailableFonts::DosisBold,
	30,
	sf::Color(255,255,255),
	sf::Color(255,255,255),
	0.0f
	}
};
const UIButton::Design UIButton::DefaultDesign::PressDesign = UIButton::Design
{
	Design::Body
	{
	UIColors::DarkYellow,
	sf::Color(0,0,0),
	0.0f
	},
	Design::Text
	{
	"Button",
	UIText::AvailableFonts::DosisBold,
	30,
	sf::Color(200,200,200),
	sf::Color(255,255,255),
	0.0f
	}
};

UIButton::UIButton(const sf::RectangleShape& body, const Design& none, const Design& hover, const Design& press, State initialState) : RectCollider(body),
m_body(body),
m_noneDesign(none),
m_hoverDesign(hover),
m_pressDesign(press),
m_currentState(initialState)
{
	// align UIText mid-center
	m_text.AlignText(UIText::TextAlign::mid_center);

	// position UIText mid-center
	m_text.PositionText(m_body);

	m_currentDesign = &m_noneDesign;
	UpdateDesign();
}

UIButton::State UIButton::GetState() const
{
	return m_currentState;
}

void UIButton::SwitchState(State newState)
{
	m_currentState = newState;
	switch (newState) {
	case State::None:
		m_currentDesign = &m_noneDesign;
		break;
	case State::Hover:
		m_currentDesign = &m_hoverDesign;
		break;
	case State::Press:
		m_currentDesign = &m_pressDesign;
		break;
	default:
		break;
	}
	UpdateDesign();
}

UIButton::Design UIButton::GetDesign() const
{
	return *m_currentDesign;
}

void UIButton::ChangeText(const std::string& newText)
{
	m_noneDesign.m_textDesign.text = newText;
	m_hoverDesign.m_textDesign.text = newText;
	m_pressDesign.m_textDesign.text = newText;
	UpdateDesign();
}

void UIButton::OnMouseEnter()
{
	m_currentState = State::Hover;
	m_currentDesign = &m_hoverDesign;
	UpdateDesign();
	SoundSystem::PlaySFX(SoundSystem::SoundType::OverSFX);
}

void UIButton::OnMouseLeave()
{
	m_currentState = State::None;
	m_currentDesign = &m_noneDesign;
	UpdateDesign();
}

void UIButton::OnMouseLeftClick()
{
	m_currentState = State::Press;
	m_currentDesign = &m_pressDesign;
	UpdateDesign();
	SoundSystem::PlaySFX(SoundSystem::SoundType::ButtonSFX);
}

void UIButton::OnMouseLeftRelease()
{
	m_currentState = State::Release;
	m_currentDesign = &m_hoverDesign;
	UpdateDesign();
}

void UIButton::UpdateDesign()
{
	//Update Body
	m_body.setFillColor(m_currentDesign->m_bodyDesign.fillColor);
	m_body.setOutlineColor(m_currentDesign->m_bodyDesign.outlineColor);
	m_body.setOutlineThickness(m_currentDesign->m_bodyDesign.outlineThickness);

	//Update Text
	m_text.setString(m_currentDesign->m_textDesign.text);
	m_text.SetFont(m_currentDesign->m_textDesign.font);
	m_text.AlignText(UIText::TextAlign::mid_center);
	m_text.setCharacterSize(m_currentDesign->m_textDesign.fontSize);
	m_text.setFillColor(m_currentDesign->m_textDesign.fontFill);
	m_text.setOutlineColor(m_currentDesign->m_textDesign.fontOutline);
	m_text.setOutlineThickness(m_currentDesign->m_textDesign.outlineThickness);
}

void UIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_body);
	target.draw(m_text);
}
