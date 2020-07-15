#include "UIResource.h"

std::vector<sf::Texture> UIResource::s_resourceTextures;

UIResource::UIResource(IToken::Type tokenType, const sf::Vector2f& position, float radius) :CircleShape(radius), m_type(tokenType)
{
	// Body
	setOrigin(radius, radius);
	setPosition(position);
	if (s_resourceTextures.empty())
	{
		LoadTextures();
	}
	setTexture(&s_resourceTextures[static_cast<int>(tokenType)]);
}

IToken::Type UIResource::GetType() const
{
	return m_type;
}

void UIResource::LoadTextures()
{
	s_resourceTextures.resize(s_texturesCount);
	size_t textureIndex = 0;
	for (auto& texture : s_resourceTextures)
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
