#include "UIToken.h"
#include "UIColors.h"
#include "SoundSystem.h"

std::vector<sf::Texture> UIToken::s_tokenTextures;

UIToken::UIToken(IToken::Type tokenType, const sf::Vector2f& position, float radius) :CircCollider(position, radius), m_type(tokenType), m_radius(radius), m_body(radius)
{
	// States
	m_numb = false;
	m_state = State::None;

	// Body
	m_body.setOrigin(radius, radius);
	m_body.setPosition(position);
	if (s_tokenTextures.empty())
	{
		LoadTextures();
	}
	m_body.setTexture(&s_tokenTextures[static_cast<int>(tokenType)]);
	m_body.setOutlineThickness(0.1f * radius);
	m_body.setOutlineColor(UIColors::Transparent);
	switch (tokenType)
	{
	case IToken::Type::GreenEmerald:
		m_hoverColor = sf::Color::Green;
		break;
	case IToken::Type::BlueSapphire:
		m_hoverColor = sf::Color::Blue;
		break;
	case IToken::Type::WhiteDiamond:
		m_hoverColor = sf::Color::White;
		break;
	case IToken::Type::BlackOnyx:
		m_hoverColor = sf::Color::Black;
		break;
	case IToken::Type::RedRuby:
		m_hoverColor = sf::Color::Red;
		break;
	case IToken::Type::Gold:
		m_hoverColor = sf::Color::Yellow;
		break;
	default:
		m_hoverColor = UIColors::NeutralWhite;
		break;
	}
}

IToken::Type UIToken::GetType() const
{
	return m_type;
}

bool UIToken::GetNumb() const
{
	return m_numb;
}

void UIToken::SetNumb(bool numb)
{
	OnMouseLeave();
	m_numb = numb;
}

UIToken::State UIToken::GetState() const
{
	return m_state;
}

void UIToken::SetState(State state)
{
	m_state = state;
}

void UIToken::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_body);
}

sf::CircleShape UIToken::GetBody()
{
	return m_body;
}

void UIToken::OnMouseEnter()
{
	if (!m_numb)
	{
		m_state = State::Hover;
		m_body.setScale(1.1, 1.1);
		m_body.setOutlineColor(m_hoverColor - UIColors::QuarterTransparent);
		SoundSystem::PlaySFX(SoundSystem::SoundType::OverSFX);
	}
}

void UIToken::OnMouseLeave()
{
	if (!m_numb)
	{
		m_state = State::None;
		m_body.setScale(1, 1);
		m_body.setOutlineColor(UIColors::Transparent);
	}
}

void UIToken::OnMouseLeftClick()
{
	if (!m_numb)
	{
		m_state = State::Press;
		m_body.setScale(0.9, 0.9);
		m_body.setOutlineColor(UIColors::Transparent);
		SoundSystem::PlaySFX(SoundSystem::SoundType::TokenSFX);
	}
}

void UIToken::OnMouseLeftRelease()
{
	if (!m_numb)
	{
		m_state = State::Release;
		m_body.setScale(1.1, 1.1);
		m_body.setOutlineColor(m_hoverColor - UIColors::QuarterTransparent);
	}
}

void UIToken::SetPosition(float x, float y)
{
	m_body.setPosition(x, y);
	ColliderBody().setPosition(x, y);
}

void UIToken::SetOrigin(float x, float y)
{
	m_body.setOrigin(x, y);
	ColliderBody().setOrigin(x, y);
}

void UIToken::SetRadius(float radius)
{
	m_body.setRadius(radius);
	ColliderBody().setRadius(radius);
}

void UIToken::LoadTextures()
{
	s_tokenTextures.resize(s_texturesCount);
	size_t textureIndex = 0;
	for (auto& texture : s_tokenTextures)
	{
		switch (static_cast<IToken::Type>(textureIndex++))
		{
		case IToken::Type::GreenEmerald:
			texture.loadFromFile(s_texturesFile + "greenEmerald.png");
			break;
		case IToken::Type::BlueSapphire:
			texture.loadFromFile(s_texturesFile + "blueSaphire.png");
			break;
		case IToken::Type::WhiteDiamond:
			texture.loadFromFile(s_texturesFile + "whiteDiamond.png");
			break;
		case IToken::Type::BlackOnyx:
			texture.loadFromFile(s_texturesFile + "blackOnyx.png");
			break;
		case IToken::Type::RedRuby:
			texture.loadFromFile(s_texturesFile + "redRuby.png");
			break;
		case IToken::Type::Gold:
			texture.loadFromFile(s_texturesFile + "gold.png");
			break;
		default:
			texture.loadFromFile(s_texturesFile + "greenEmerald.png");
			break;
		}
	}
}
