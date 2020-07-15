#pragma once
#include <SFML/Graphics.hpp>

#include "IToken.h"

class UIResource :public sf::CircleShape
{
public:
	// Constr.
	UIResource(IToken::Type tokenType, const sf::Vector2f& position = sf::Vector2f(0, 0), float radius = 50);

	// G&S
	IToken::Type GetType() const;

private:
	static void LoadTextures();
	
private:
	// Textures
	const inline static std::string s_texturesFile = "../external/Resources/Textures/Resources/";
	static std::vector<sf::Texture> s_resourceTextures;
	static inline const size_t s_texturesCount = 5;
	
private:
	IToken::Type m_type;
};

