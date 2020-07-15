#pragma once
#include "CircCollider.h"
#include "IToken.h"

#include <SFML/Graphics.hpp>

class UIToken :public CircCollider, public sf::Drawable
{
public:
	enum class State
	{
		None,
		Hover,
		Press,
		Release
	};

public:
	// Constr.
	UIToken(IToken::Type tokenType, const sf::Vector2f& position = sf::Vector2f(0, 0), float radius = 50);

	// G&S
	IToken::Type GetType() const;
	bool GetNumb() const;
	void SetNumb(bool numb);
	State GetState() const;
	void SetState(State state);

	// Graphics
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::CircleShape GetBody();

	// Collider
	void OnMouseEnter() override;
	void OnMouseLeave() override;
	void OnMouseLeftClick() override;
	void OnMouseLeftRelease() override;

	// Manipulators
	void SetPosition(float x, float y);
	void SetOrigin(float x, float y);
	void SetRadius(float radius);


private:
	static void LoadTextures();

private:
	// Textures
	const inline static std::string s_texturesFile = "../external/Resources/Textures/Tokens/";
	static std::vector<sf::Texture> s_tokenTextures;
	static inline const size_t s_texturesCount = 6;

private:
	IToken::Type m_type;
	State m_state;
	bool m_numb;
	float m_radius;
	sf::CircleShape m_body;
	sf::Color m_hoverColor;
};

